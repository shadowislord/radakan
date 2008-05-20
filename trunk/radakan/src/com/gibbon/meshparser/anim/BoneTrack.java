package com.gibbon.meshparser.anim;

import com.jme.math.Quaternion;
import com.jme.math.TransformMatrix;

public class BoneTrack {

    private TransformMatrix[] frames;
    private float[] times;
    
    private Bone target;
    
    private Quaternion temp1 = new Quaternion();
    private Quaternion temp2 = new Quaternion();
    
    public BoneTrack(Bone target){
        this.target = target;
    }
    
    public void setData(float[] times, TransformMatrix[] transforms){
        this.times = times;
        this.frames = transforms;
    }
    
    public void setTime(float time) {
        int startFrame = 0;
        
        // by default the mode is to clamp for times beyond the current timeline
        if (time < times[0]){
            target.animTransform.set(frames[0]);
        }else if (time > times[times.length-1]){
            target.animTransform.set(frames[times.length-1]);
        } else{
            for (int i = 0; i < times.length; i++){
                if (times[i] < time)
                    startFrame = i;
            }
            
            int endFrame = startFrame + 1;
            
            float blend = (time - times[startFrame]) / (times[endFrame] - times[startFrame]);
            
            target.animTransform.interpolateTransforms(frames[startFrame], 
                                                       frames[endFrame], 
                                                       blend, temp1, temp2);
        }
    }
    
}
