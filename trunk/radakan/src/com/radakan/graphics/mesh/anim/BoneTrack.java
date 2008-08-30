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
import com.jme.math.Vector3f;

public class BoneTrack {

    private Vector3f[] translations;
    private Quaternion[] rotations;
    private float[] times;
    
    private Bone target;
    
    private Vector3f tempV = new Vector3f();
    private Quaternion tempQ = new Quaternion();
    
    public BoneTrack(Bone target){
        this.target = target;
    }
    
    public void checkValid(){
        if (  (times.length != translations.length)
           || (rotations.length != times.length)){
            assert false;
        }
        
        for (int i = 0; i < times.length-1; i++)
            assert times[i] < times[i+1];
    }
    
    public void setData(float[] times, Vector3f[] translations, Quaternion[] rotations){
        if (times.length == 0)
            throw new RuntimeException("BoneTrack with no keyframes!");
        
        this.times = times;
        this.translations = translations;
        this.rotations = rotations;
    }
    
    public void setTime(float time) {
        int startFrame = 0;
        
        try{
            // by default the mode is to clamp for times beyond the current timeline
            if (time < times[0]){
                target.setAnimTransforms(translations[0], rotations[0]);
            }else if (time > times[times.length-1]){
                target.setAnimTransforms(translations[translations.length-1], 
                                         rotations[rotations.length-1]);
            } else{
                for (int i = 0; i < times.length; i++){
                    if (times[i] < time){
                        startFrame = i;
                    }
                }

                int endFrame = startFrame + 1;
                if (times.length == endFrame){
                    endFrame = startFrame;
                }

                float blend = (time - times[startFrame]) / (times[endFrame] - times[startFrame]);

                tempQ.slerp(rotations[startFrame], rotations[endFrame], blend);
                tempV.interpolate(translations[startFrame], translations[endFrame], blend);
//                
//                if (blend < 0.5f){
//                    target.setAnimTransforms(translations[startFrame],
//                                             rotations[startFrame]);
//                }else{
//                    target.setAnimTransforms(translations[endFrame],
//                                             rotations[endFrame]);
//                }
//                
                target.setAnimTransforms(tempV, tempQ);
                                             
                
//                target.animMat.interpolateTransforms(frames[startFrame], 
//                                                     frames[endFrame], 
//                                                     blend, temp1, temp2);
            }
        } catch (Throwable t){
            t.printStackTrace();
        }
    }
    
}
