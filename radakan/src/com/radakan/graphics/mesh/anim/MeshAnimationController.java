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

import com.jme.math.Matrix4f;
import com.jme.math.Vector3f;
import java.util.Map;

import com.jme.scene.Controller;
import com.jme.scene.state.GLSLShaderObjectsState;
import com.jme.scene.state.RenderState;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.util.Collection;

public class MeshAnimationController extends Controller {

    private static final long serialVersionUID = -2412532346418342259L;

    private static final boolean forceSWskinning = true;
    
    private final OgreMesh[] targets;
    private final Skeleton skeleton;
    private final Map<String, Animation> animationMap;
    
    private Animation animation;
    private float time = 0f;
    private boolean resetToBindEveryFrame = false;
    
    private int framesToSkip = 0;
    private int curFrame = 0;

    public MeshAnimationController(OgreMesh[] meshes,
                                   Skeleton skeleton,
                                   Map<String, Animation> anims){

        this.setRepeatType(RT_WRAP);
        this.skeleton = skeleton;
        this.animationMap = anims;
        this.targets = meshes;

        for (int i = 0; i < targets.length; i++){
            // does this mesh has any pose/morph animation tracks?
            for (Animation anim : animationMap.values()){
                MeshAnimation manim = anim.getMeshAnimation();
                BoneAnimation banim = anim.getBoneAnimation();

                if (manim != null){
                    for (Track t : manim.getTracks()){
                        if (t.getTargetMeshIndex() == i){
                            targets[i].clearBindPose();
                            targets[i].saveCurrentToBindPose();
                            break;
                        }
                    }
                }

                
                if (banim != null && !isHardwareSkinning()){
                    targets[i].clearBindPose();
                    targets[i].saveCurrentToBindPose();
                    break;
                }
            }
            
            if (targets[i].getWeightBuffer() != null)
                targets[i].getWeightBuffer().initializeWeights();
        }
        
        if (isHardwareSkinning()){
            assignShaderLogic();
        }
        
        reset();
    }

    public MeshAnimationController(OgreMesh[] meshes, MeshAnimationController sourceControl){
        this.setRepeatType(RT_WRAP);
        this.skeleton = new Skeleton(sourceControl.skeleton);
        this.animationMap = sourceControl.animationMap;
        this.targets = meshes;
        
        if (isHardwareSkinning()){
            assignShaderLogic();
        }
        
        reset();
    }
    
    public Bone getBone(String name){
        return skeleton.getBone(name);
    }
    
    public void setAnimation(String name){
        if (name.equals("<bind>")){
            reset();
            return;
        }
        
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
            return -1;
        
        return anim.getLength();
    }
    
    public String getActiveAnimation(){
        return animation.getName();
    }
    
    public Collection<String> getList(){
        return animationMap.keySet();
    }
    
    public void setFrameSkip(int framesToSkip){
        this.framesToSkip = framesToSkip;
    }
    
    public void setTime(float time){
        this.time = time;
    }
    
    Skeleton getSkeleton(){
        return skeleton;
    }
    
    OgreMesh[] getMeshList(){
        return targets;
    }
    
    void reset(){
        resetToBind();
        skeleton.getRoot().reset();
        skeleton.getRoot().update();
        resetToBindEveryFrame = false;
        animation = null;
        time = 0;
    }
    
    void resetToBind(){
        for (int i = 0; i < targets.length; i++){
            if (targets[i].hasBindPose()){
                targets[i].restoreBindPose();
            }
        }
    }
    
    private void assignShaderLogic(){
        SkinningShaderLogic logic = new SkinningShaderLogic();
        for (OgreMesh target : targets){
            GLSLShaderObjectsState glsl = (GLSLShaderObjectsState) target.getRenderState(RenderState.RS_GLSL_SHADER_OBJECTS);
            if (glsl == null){
                glsl = BoneAnimationLoader.createSkinningShader(skeleton.getBoneCount(),
                                                                target.getWeightBuffer().maxWeightsPerVert);
                target.setRenderState(glsl);
            }
            glsl.setShaderDataLogic(logic);
        }
    }
    
    public boolean isHardwareSkinning(){
        return !forceSWskinning && GLSLShaderObjectsState.isSupported();
    }
    
    private void softwareSkinUpdate(OgreMesh mesh){
        Vector3f vertex = new Vector3f();
        Vector3f normal = new Vector3f();
        Vector3f temp = new Vector3f();
        Vector3f resultVert = new Vector3f();
        Vector3f resultNorm = new Vector3f();
        
        Matrix4f offsetMatrices[] = skeleton.computeSkinningMatrices();
        
        // NOTE: This code assumes the vertex buffer is in bind pose
        // resetToBind() has been called this frame
        FloatBuffer vb = mesh.getVertexBuffer();
        vb.rewind();
        
        FloatBuffer nb = mesh.getNormalBuffer();
        nb.rewind();

        // get boneIndexes and weights for mesh
        ByteBuffer ib = mesh.getWeightBuffer().indexes;
        FloatBuffer wb = mesh.getWeightBuffer().weights;
        int maxWeightsPerVert = mesh.getWeightBuffer().maxWeightsPerVert;
        int fourMinusMaxWeights = 4 - maxWeightsPerVert;
        ib.rewind();
        wb.rewind();
        

        // iterate vertices and apply skinning transform for each effecting bone
        for (int vert = 0; vert < mesh.getVertexCount(); vert++){
            vertex.set(vb.get(), vb.get(), vb.get());
            normal.set(nb.get(), nb.get(), nb.get());
            resultVert.zero();
            resultNorm.zero();
            
            for (int w = 0; w < maxWeightsPerVert; w++){
                float weight = wb.get();

                Matrix4f offsetMatrix = offsetMatrices[ib.get()];

                temp.set(vertex);
                offsetMatrix.mult(temp, temp);
                resultVert.x += temp.x * weight;
                resultVert.y += temp.y * weight;
                resultVert.z += temp.z * weight;

                temp.set(normal);
                offsetMatrix.rotateVect(temp);
                resultNorm.x += temp.x * weight;
                resultNorm.y += temp.y * weight;
                resultNorm.z += temp.z * weight;
                
                ib.position(ib.position()+fourMinusMaxWeights);
                wb.position(wb.position()+fourMinusMaxWeights);
            }

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
        
        if (framesToSkip > 0){
            // check frame skipping
            curFrame++;
            
            if (curFrame != framesToSkip){
                time += tpf * getSpeed();
                return;
            }else{
                curFrame = 0;
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
        animation.setTime(time, targets, skeleton);
        
        if (animation.hasBoneAnimation()){
            // this sets the SKINNING MATRIX field of ALL bones
            skeleton.getRoot().update();
            
            if (!isHardwareSkinning()){
                // here update the targets verticles if no hardware skinning supported
                
                // if hardware skinning is supported, the matrices and weight buffer
                // will be sent by the SkinningShaderLogic object assigned to the shader
                for (int i = 0; i < targets.length; i++){
                    softwareSkinUpdate(targets[i]);
                }
            }
        }
        
        // increment the time
        time += tpf * getSpeed();
    }
    
    
}
