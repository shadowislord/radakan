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

import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;

public  final class BoneTrack {

    private final int targetBoneIndex;
    private final Vector3f[] translations;
    private final Quaternion[] rotations;
    private final float[] times;

    // temp vectors for interpolation
    private final Vector3f tempV = new Vector3f();
    private final Quaternion tempQ = new Quaternion();
    
    public BoneTrack(int targetBoneIndex, float[] times, Vector3f[] translations, Quaternion[] rotations){
        this.targetBoneIndex = targetBoneIndex;
        
        if (times.length == 0)
            throw new RuntimeException("BoneTrack with no keyframes!");
        
        this.times = times;
        this.translations = translations;
        this.rotations = rotations;
    }
    
    public void setTime(float time, Skeleton skeleton) {
        Bone target = skeleton.getBone(targetBoneIndex);
        
        // by default the mode is to clamp for times beyond the current timeline
        if (time < times[0]){
            target.setAnimTransforms(translations[0], rotations[0]);
        }else if (time > times[times.length-1]){
            target.setAnimTransforms(translations[translations.length-1], 
                                     rotations[rotations.length-1]);
        } else{
            int startFrame = 0;
            int endFrame = 0;
            
            for (int i = 0; i < times.length; i++){
                if (times[i] <= time){
                    startFrame = i;
                    endFrame   = i + 1;
                    if (times[startFrame] >= times[endFrame]){
                        throw new IllegalStateException("Cannot happen");
                    }
                }
            }

            
//            if (times.length == endFrame){
//                endFrame = startFrame;
//            }
            
            float blend = (time - times[startFrame]) / (times[endFrame] - times[startFrame]);
            blend = FastMath.clamp(blend, 0f, 1f);
            
            tempQ.slerp(rotations[startFrame], rotations[endFrame], blend);
            tempV.interpolate(translations[startFrame], translations[endFrame], blend);

            target.setAnimTransforms(tempV, tempQ);
        }
    }
    
}
