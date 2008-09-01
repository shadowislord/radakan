/*
 * Radakan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.radakan.graphics.mesh.anim;

import com.jme.math.Matrix3f;
import com.jme.math.Matrix4f;
import com.jme.math.Quaternion;
import com.jme.math.TransformMatrix;
import com.jme.math.TransformQuaternion;
import com.jme.math.Vector3f;
import java.util.Map;

import com.jme.scene.Controller;
import com.jme.scene.Geometry;
import com.jme.scene.state.GLSLShaderDataLogic;
import com.jme.scene.state.GLSLShaderObjectsState;
import com.jme.scene.state.RenderState;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.util.Collection;
import java.util.HashMap;

public class MeshAnimationController extends Controller {

    private static final long serialVersionUID = -2412532346418342259L;

    private static final boolean forceSWskinning = true;
    
    private OgreMesh[] targets;
    private Skeleton skeleton;
    
    private Map<String, Animation> animationMap;
    private Animation animation;
    private float time = 0f;
    
    private boolean resetToBindEveryFrame = false;
    private boolean hardwareSkinning;

    private class SkinningShaderLogic implements GLSLShaderDataLogic {
        public void applyData(GLSLShaderObjectsState shader, Geometry geom) {
            // send bone data
            skeleton.sendToShader(shader);
            
            // send weight buffer (indexes + weights)
            ((OgreMesh)geom).getWeightBuffer().sendToShader(shader);  
        }
    }
    
    public MeshAnimationController(OgreMesh[] meshes,
                                   Skeleton skeleton,
                                   Map<String, Animation> anims){
        if (anims == null)
            anims = new HashMap<String, Animation>();
        
        this.setRepeatType(RT_WRAP);
        
        hardwareSkinning = !forceSWskinning && GLSLShaderObjectsState.isSupported();
        
        this.skeleton = skeleton;
        animationMap = anims;
        targets = meshes;
 
        // what a messy loop..
        // finds which meshes need their pose (vertex buffer)
        // saved as a copy.
        // if the mesh data is going to be modified by the CPU
        // then a copy must be made.
        // a target's mesh will be modified in two cases:
        // 1) if it has mesh animations
        // 2) if it has bone animations and hardware skinning is not supported
        for (int i = 0; i < targets.length; i++){
            // does this mesh has any pose/morph animation tracks?
            animsearch: {
                for (Animation anim : animationMap.values()){
                    MeshAnimation manim = anim.getMeshAnimation();
                    BoneAnimation banim = anim.getBoneAnimation();
                    
                    if (manim != null){
                        for (Track t : manim.getTracks()){
                            if (t.getTarget() == targets[i]){
                                targets[i].clearBindPose();
                                targets[i].saveCurrentToBindPose();
                                break animsearch;
                            }
                        }
                    }
                    
                    if (banim != null && !isHardwareSkinning()){
                        targets[i].clearBindPose();
                        targets[i].saveCurrentToBindPose();
                        break animsearch;
                    }  
                }
            }
        }
        
        if (isHardwareSkinning()){
            assignShaderLogic();
        }
    }

    private void assignShaderLogic(){
        SkinningShaderLogic logic = new SkinningShaderLogic();
        for (OgreMesh target : targets){
            GLSLShaderObjectsState glsl = (GLSLShaderObjectsState) target.getRenderState(RenderState.RS_GLSL_SHADER_OBJECTS);
            if (glsl == null){
                glsl = BoneAnimationLoader.createSkinningShader();
                target.setRenderState(glsl);
            }
            glsl.setShaderDataLogic(logic);
        }
    }
    
    public boolean isHardwareSkinning(){
        return hardwareSkinning;
    }
    
    public void setAnimation(String name){
        animation = animationMap.get(name);
        
        if (animation == null)
            throw new NullPointerException();
        
        resetToBind();
        resetToBindEveryFrame = animation.hasMeshAnimation() || !isHardwareSkinning();
        
        time = 0;
    }
    
    public float getAnimationLength(String name){
        Animation anim = animationMap.get(name);

        if (anim == null)
            throw new NullPointerException();
        
        return anim.getLength();
    }
    
    public String getActiveAnimation(){
        return animation.getName();
    }
    
    public Collection<String> getList(){
        return animationMap.keySet();
    }
    
    public void reset(){
        resetToBind();
        animation = null;
    }
    
    private void resetToBind(){
        for (int i = 0; i < targets.length; i++){
            if (targets[i].hasBindPose()){
                targets[i].restoreBindPose();
            }
        }
    }
    
    private void hardwareSkinUpdate(OgreMesh mesh){
        GLSLShaderObjectsState glsl = 
                            (GLSLShaderObjectsState) 
                                    mesh.getRenderState(RenderState.RS_GLSL_SHADER_OBJECTS);
                    
        if (glsl != null){
            // update attribute and uniform variables
            // for each mesh
            skeleton.sendToShader(glsl);
            mesh.getWeightBuffer().sendToShader(glsl);
        }
    }
    
    private void softwareSkinUpdate(OgreMesh mesh){
        Vector3f vertex = new Vector3f();
        Vector3f normal = new Vector3f();
        Vector3f temp = new Vector3f();
        Vector3f resultVert = new Vector3f();
        Vector3f resultNorm = new Vector3f();
        
        Matrix4f offsetMatrix = new Matrix4f();
        
        // NOTE: This code assumes the vertex buffer is in bind pose
        // resetToBind() has been called this frame
        FloatBuffer vb = mesh.getVertexBuffer();
        vb.rewind();
        
        FloatBuffer nb = mesh.getNormalBuffer();
        nb.rewind();

        // get boneIndexes and weights for mesh
        ByteBuffer ib = mesh.getWeightBuffer().indexes;
        FloatBuffer wb = mesh.getWeightBuffer().weights;
        ib.rewind();
        wb.rewind();

        // iterate vertices and apply skinning transform for each effecting bone
        for (int vert = 0; vert < mesh.getVertexCount(); vert++){
            vertex.set(vb.get(), vb.get(), vb.get());
            normal.set(nb.get(), nb.get(), nb.get());
            resultVert.zero();
            resultNorm.zero();
            
            // small check to make sure weight sum = 1
            //float weightSum = 0.0f;
            for (int w = 0; w < 4; w++){
                float weight = wb.get();
                //weightSum += weight;

                Bone b = skeleton.getBone(ib.get());
                b.getOffsetTransform(offsetMatrix);

                if (weight > 0.01f){
                    temp.set(vertex);
                    offsetMatrix.mult(temp, temp);
                    temp.multLocal(weight);
                    resultVert.addLocal(temp);

                    temp.set(normal);
                    offsetMatrix.rotateVect(temp);
                    temp.multLocal(weight);
                    resultNorm.addLocal(temp);
                }
            }

//            if (weightSum != 1.0f)
//                throw new RuntimeException("Weight sum for vert "+vert+" is "+ weightSum +" not 1!");

            resultNorm.normalizeLocal();
            
            // overwrite vertex with transformed pos
            vb.position(vb.position()-3);
            vb.put(resultVert.x).put(resultVert.y).put(resultVert.z);
            
            nb.position(nb.position()-3);
            nb.put(resultNorm.x).put(resultNorm.y).put(resultNorm.z);
        }
        
        vb.flip();
        nb.flip();
        
        mesh.setHasDirtyVertices(true);
        mesh.updateModelBound();
    }
    
    @Override
    public void update(float tpf) {
        if (!isActive() || animation == null)
            return;
        
        // do clamping/wrapping of time
        if (time < 0f){
            switch (getRepeatType()){
                case RT_CLAMP:
                    time = 0f;
                    break;
                case RT_CYCLE:
                    time = 0f;
                    break;
                case RT_WRAP:
                    time = animation.getLength() - time;
                    break;
            }
        }else if (time > animation.getLength()){
            switch (getRepeatType()){
                case RT_CLAMP:
                    time = animation.getLength();
                    break;
                case RT_CYCLE:
                    time = animation.getLength();
                    break;
                case RT_WRAP:
                    time = time - animation.getLength();
                    break;
            }
        }
        
        // reset the meshes verticles to bind position
        if (resetToBindEveryFrame)
            resetToBind();
        
        // apply the tracks for the current time
        if (animation.hasBoneAnimation()){
            // reset all bones ANIMATION MATRIX field to identity (zero transform)
            skeleton.getRoot().reset();
        }
        
        // BoneAnimation: this sets the ANIMATION MATRIX field of bones ANIMATED in the tracks
        animation.setTime(time);
        
        if (animation.hasBoneAnimation()){
            // this sets the SKINNING MATRIX field of ALL bones
            skeleton.getRoot().update();
            
            if (isHardwareSkinning()){
                for (int i = 0; i < targets.length; i++){
                    hardwareSkinUpdate(targets[i]);
                }
            }else{
                // here update the targets verticles if no hardware skinning supported
                for (int i = 0; i < targets.length; i++){
                    softwareSkinUpdate(targets[i]);
                }
            }
        }
        
        //assert time == oldTime;
        
        // increment the time
        time += tpf * getSpeed();
    }
    
    
}
