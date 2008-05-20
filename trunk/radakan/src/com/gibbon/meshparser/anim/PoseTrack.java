package com.gibbon.meshparser.anim;

import com.gibbon.meshparser.*;
import com.jme.scene.TriMesh;

public class PoseTrack extends Track {

    private PoseFrame[] frames;
    private float[]     times;
    
    public static class PoseFrame { 
        Pose[]  poses;
        float[] weights;
    }
    
    public void setData(float[] times, PoseFrame[] frames){
        this.times = times;
        this.frames = frames;
    }
            
            
    public PoseTrack(TriMesh target){
        super(target);
    }
    
    private void applyFrame(int frameIndex, float weight){
        PoseFrame frame = frames[frameIndex];
        
        for (int i = 0; i < frame.poses.length; i++){
            Pose pose = frame.poses[i];
            float poseWeight = frame.weights[i] * weight;
            
            pose.apply(poseWeight, target.getVertexBuffer());
        }
        
        target.setHasDirtyVertices(true);
    }
    
    @Override
    public void setTime(float time) {
        int startFrame = 0;
        
        if (time < times[0]){
            applyFrame(0, 1f);
        }else if (time > times[times.length-1]){
            applyFrame(times.length-1, 1f);
        } else{
            for (int i = 0; i < times.length; i++){
                if (times[i] < time)
                    startFrame = i;
            }
            
            int endFrame = startFrame + 1;
            float blend = (time - times[startFrame]) / (times[endFrame] - times[startFrame]);
            applyFrame(startFrame, blend);
            applyFrame(endFrame,   1-blend);
        }
    }

}
