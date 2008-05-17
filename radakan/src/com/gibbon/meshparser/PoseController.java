package com.gibbon.meshparser;

import com.jme.scene.Controller;
import com.jme.scene.TriMesh;
import com.jme.util.geom.BufferUtils;
import java.nio.FloatBuffer;
import java.util.Map;

public class PoseController extends Controller {

    private Map<String, Pose> poseMap;
    private FloatBuffer originalVertBuf;
    private TriMesh target;
    
    private Pose curPose = null;
    private float blendTime = 0f;
    private float blendTimeMax = 0f;
    
    private static class TriMeshEntry {
        TriMesh mesh;
        FloatBuffer originalVertBuf;
    }
    
//    public PoseController(Map<TriMesh, List<Pose>> trimeshPoses){
//        poseMap = poses;
//        
//        this.target = target;
//        
//        // create a copy of the original vertex buffer
//        FloatBuffer meshVertBuf = target.getVertexBuffer();
//        originalVertBuf = BufferUtils.createFloatBuffer(meshVertBuf.capacity());
//        meshVertBuf.rewind();
//        originalVertBuf.rewind();
//        originalVertBuf.put(meshVertBuf);
//        originalVertBuf.flip();
//    }
//    
    @Override
    public void update(float time) {
        if (!isActive())
            return;
        
        if (curPose != null && blendTime > 0f){
            float blend = blendTime / blendTimeMax;
            
            resetToBind();
            curPose.apply(blend, target.getVertexBuffer());
            
            blendTime -= time;
            if (blendTime < 0f)
                blendTime = 0f;
        }
    }
    
    private void resetToBind(){
        FloatBuffer meshVertBuf = target.getVertexBuffer();
        meshVertBuf.rewind();
        meshVertBuf.put(originalVertBuf);
    }
    
    public void reset(){
        resetToBind();
        curPose = null;
        blendTime = 0f;
        blendTimeMax = 0f;
    }
    
    public void blendToPose(String name, float time){
        curPose = poseMap.get(name);
        
        if (curPose == null)
            throw new NullPointerException();
        
        blendTime = time;
        blendTimeMax = time;
    }
    
    public void setPose(String name){
        curPose = poseMap.get(name);
        
        if (curPose == null)
            throw new NullPointerException();
        
        blendTime = 0f;
        blendTimeMax = 0f;
        
        resetToBind();
        curPose.apply(1f, target.getVertexBuffer());
    }
    
    
    
}
