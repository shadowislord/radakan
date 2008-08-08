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
