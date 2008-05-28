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

package com.gibbon.meshparser.anim;

import com.gibbon.meshparser.*;
import com.jme.scene.Controller;
import com.jme.scene.Geometry;
import com.jme.scene.state.GLSLShaderDataLogic;
import com.jme.scene.state.GLSLShaderObjectsState;
import com.jme.scene.state.RenderState;
import java.util.Map;

public class MeshAnimationController extends Controller {

    private static boolean forceSWskinning = false;
    
    private OgreMesh[] targets;
    private Skeleton skeleton;
    
    private Map<String, Animation> animationMap;
    private Animation animation;
    private float time = 0f;
    
    private boolean resetToBindEveryFrame = false;

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
    }

    private void assignShaderLogic(){
        SkinningShaderLogic logic = new SkinningShaderLogic();
        for (OgreMesh target : targets){
            GLSLShaderObjectsState glsl = (GLSLShaderObjectsState) target.getRenderState(RenderState.RS_GLSL_SHADER_OBJECTS);
            if (glsl == null){
                glsl = MeshAnimationLoader.createSkinningShader();
                target.setRenderState(glsl);
            }
            glsl.setShaderDataLogic(logic);
        }
    }
    
    public boolean isHardwareSkinning(){
        return !forceSWskinning && GLSLShaderObjectsState.isSupported();
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
        animation.setTime(time);
        
        if (animation.hasBoneAnimation()){
            skeleton.updateWorldTransforms();
            
            if (isHardwareSkinning()){
                for (int i = 0; i < targets.length; i++){
                    GLSLShaderObjectsState glsl = 
                            (GLSLShaderObjectsState) 
                                    targets[i].getRenderState(RenderState.RS_GLSL_SHADER_OBJECTS);
                    
                    if (glsl != null){
                        // update attribute and uniform variables
                        // for each mesh
                        skeleton.sendToShader(glsl);
                        targets[i].getWeightBuffer().sendToShader(glsl);
                    }
                }
            }else{
                // FIXME:
                // here we update the targets verticles if no hardware skinning supported
            }
        }
        
        // increment the time
        time += tpf * getSpeed();
    }
    
    
}
