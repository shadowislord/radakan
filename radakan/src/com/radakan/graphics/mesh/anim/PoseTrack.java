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
public final class PoseTrack extends Track {

    private PoseFrame[] frames;
    private float[]     times;
    
    public static class PoseFrame {
        
        public PoseFrame(Pose[] poses, float[] weights){
            this.poses = poses;
            this.weights = weights;
        }
        
        final Pose[]  poses;
        final float[] weights;
        
    }
    
    public PoseTrack(int targetMeshIndex, float[] times, PoseFrame[] frames){
        super(targetMeshIndex);
        this.times = times;
        this.frames = frames;
    }
    
    private void applyFrame(OgreMesh target, int frameIndex, float weight){
        PoseFrame frame = frames[frameIndex];
        
        for (int i = 0; i < frame.poses.length; i++){
            Pose pose = frame.poses[i];
            float poseWeight = frame.weights[i] * weight;
            
            pose.apply(poseWeight, target.getVertexBuffer());
        }
        
        target.setHasDirtyVertices(true);
    }
    
    @Override
    public void setTime(float time, OgreMesh[] targets) {
        OgreMesh target = targets[targetMeshIndex];
        if (time < times[0]){
            applyFrame(target, 0, 1f);
        }else if (time > times[times.length-1]){
            applyFrame(target, times.length-1, 1f);
        } else{
            int startFrame = 0;
            for (int i = 0; i < times.length; i++){
                if (times[i] < time)
                    startFrame = i;
            }
            
            int endFrame = startFrame + 1;
            float blend = (time - times[startFrame]) / (times[endFrame] - times[startFrame]);
            applyFrame(target, startFrame, blend);
            applyFrame(target, endFrame,   1-blend);
        }
    }

}
