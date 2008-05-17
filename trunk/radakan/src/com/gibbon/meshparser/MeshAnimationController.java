package com.gibbon.meshparser;

import com.jme.scene.Controller;
import com.jme.scene.TriMesh;
import com.jme.util.geom.BufferUtils;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class MeshAnimationController extends Controller {

    private TriMesh[] targets;
    private FloatBuffer[] originalVertBufs;
    private Map<String, MeshAnimation> animationMap;
    private MeshAnimation animation = null;
    private float time = 0f;
    
    public MeshAnimationController(Map<String, MeshAnimation> animations){
        animationMap = animations;
        
        // collect the targets of this mesh animation controller from
        // the animations
        List<TriMesh> targetsList = new ArrayList<TriMesh>();
        List<FloatBuffer> vertBufsList = new ArrayList<FloatBuffer>();
        
        for (MeshAnimation anim : animations.values()){
            for (Track track : anim.getTracks()){
                if (!targetsList.contains(track.getTarget())){
                    // create a copy of the mesh verticles
                    FloatBuffer meshVertBuf = track.getTarget().getVertexBuffer();
                    FloatBuffer originalVertBuf = BufferUtils.createFloatBuffer(meshVertBuf.capacity());
                    
                    meshVertBuf.rewind();
                    originalVertBuf.rewind();
                    
                    originalVertBuf.put(meshVertBuf);
                    
                    vertBufsList.add(originalVertBuf);
                    targetsList.add(track.getTarget());
                }
            }
        }
        
        targets = targetsList.toArray(new TriMesh[0]);
        originalVertBufs = vertBufsList.toArray(new FloatBuffer[0]);
    }

    public void setAnimation(String name){
        animation = animationMap.get(name);
        
        if (animation == null)
            throw new NullPointerException();
        
        time = 0;
    }
    
    public float getAnimationLength(String name){
        MeshAnimation anim = animationMap.get(name);
        
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
