package com.gibbon.meshparser.anim;

import com.gibbon.meshparser.*;
import com.jme.scene.Controller;
import com.jme.scene.TriMesh;
import com.jme.util.geom.BufferUtils;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MeshAnimationController extends Controller {

    private TriMesh[] targets;
    private FloatBuffer[] originalVertBufs;
    private WeightBuffer[] weightBufs;
    
    private Map<String, Animation> animationMap;
    
    private Animation animation;
    
    private float time = 0f;
    
    public MeshAnimationController(TriMesh[] targets,
                                   WeightBuffer[] weightBufs,
                                   Map<String, MeshAnimation> meshAnims,
                                   Map<String, BoneAnimation> boneAnims){
        
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
        
        this.targets = targets;
        this.weightBufs = weightBufs;
        originalVertBufs = new FloatBuffer[targets.length];
        
        for (int i = 0; i < targets.length; i++){
            FloatBuffer meshVertBuf = targets[i].getVertexBuffer();
            FloatBuffer originalVertBuf = BufferUtils.createFloatBuffer(meshVertBuf.capacity());

            meshVertBuf.rewind();
            originalVertBuf.rewind();

            originalVertBuf.put(meshVertBuf);
            
            originalVertBufs[i] = originalVertBuf;
        }
        
    }

    public void setAnimation(String name){
        animation = animationMap.get(name);
        
        if (animation == null)
            throw new NullPointerException();
        
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
            FloatBuffer meshVertBuf = targets[i].getVertexBuffer();
            FloatBuffer originalVertBuf = originalVertBufs[i];
        
            meshVertBuf.rewind();
            originalVertBuf.rewind();
            
            meshVertBuf.put(originalVertBuf);
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
        resetToBind();
        
        // apply the tracks for the current time
        animation.setTime(time);
        
        // increment the time
        time += tpf * getSpeed();
    }
    
    
}
