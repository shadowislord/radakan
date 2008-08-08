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

import com.jme.scene.TriMesh;

/**
 * A single track of pose animation associated with a certain mesh.
 */
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
