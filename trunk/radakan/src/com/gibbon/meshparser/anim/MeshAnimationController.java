package com.gibbon.meshparser.anim;

import com.gibbon.meshparser.*;
import com.jme.scene.Controller;
import com.jme.scene.Geometry;
import com.jme.scene.TriMesh;
import com.jme.scene.state.GLSLShaderDataLogic;
import com.jme.scene.state.GLSLShaderObjectsState;
import com.jme.scene.state.RenderState;
import java.nio.FloatBuffer;
import java.util.HashMap;
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
            skeleton.sendToShader(shader);
            ((OgreMesh)geom).getWeightBuffer().sendToShader(shader);  
        }
    }
    
    public MeshAnimationController(OgreMesh[] meshes,
                                   Skeleton skeleton,
                                   Map<String, MeshAnimation> meshAnims,
                                   Map<String, BoneAnimation> boneAnims){
        
        this.skeleton = skeleton;
        animationMap = new HashMap<String, Animation>();
        
        for (BoneAnimation banim : boneAnims.values()){
            if (!animationMap.containsKey(banim.getName())){
                MeshAnimation manim = meshAnims.get(banim.getName());
                Animation anim = new Animation(banim, manim);
                animationMap.put(banim.getName(), anim);
            }
        }
        
        for (MeshAnimation manim : meshAnims.values()){
            if (!animationMap.containsKey(manim.getName())){
                animationMap.put(manim.getName(), new Animation(null, manim));
            }
        }
        
        this.targets = meshes;

        SkinningShaderLogic logic = null;
        if (isHardwareSkinning()){
            logic = new SkinningShaderLogic();
        }
        
        for (int i = 0; i < targets.length; i++){
            // does this mesh has any pose/morph animation tracks?
            boolean hasMeshAnim = false;
            animsearch: for (MeshAnimation anim : meshAnims.values()){
                for (Track t : anim.getTracks()){
                    if (t.getTarget() == targets[i]){
                        hasMeshAnim = true;
                        break animsearch;
                    }
                }
            }
            
            if (isHardwareSkinning()){
                GLSLShaderObjectsState glsl = (GLSLShaderObjectsState) targets[i].getRenderState(RenderState.RS_GLSL_SHADER_OBJECTS);
                glsl.setShaderDataLogic(logic);
            }
        }
        
        
    }

    public boolean isHardwareSkinning(){
        return !forceSWskinning && GLSLShaderObjectsState.isSupported();
    }
    
    public void setAnimation(String name){
        animation = animationMap.get(name);
        
        if (animation == null)
            throw new NullPointerException();
        
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
            }
        }
        
        // increment the time
        time += tpf * getSpeed();
    }
    
    
}
