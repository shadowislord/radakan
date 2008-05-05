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

package com.gibbon.meshparser;

import com.jme.math.Matrix4f;
import com.jme.scene.Controller;

public class SkeletonController extends Controller {

    private Bone rootBone;
    private Bone[] boneList;
    private float currentTime = 0f;
    private Animation activeAnimation;
    
    private Matrix4f transformTemp = new Matrix4f();
    private Matrix4f[] boneWorldTransforms;
    private static final Matrix4f NULL_TRANSFORM = new Matrix4f();
    
    static {
        NULL_TRANSFORM.loadIdentity();
    }
    
    private void updateBone(Bone bone, Keyframe[] frames, int frame){
        int currentFrame = frame;
        Matrix4f transform;
        
        if (frames.length == 1){
            transform = frames[0].transform;
        }else if (frames.length == 0){
            transform = NULL_TRANSFORM;
        }else{
            // calculate transform based on keyframes
            while (frames.length-1 > currentFrame && frames[currentFrame+1].time < currentTime){
                currentFrame++;
            }

            Keyframe startFrame = frames[currentFrame];
            Keyframe endFrame = frames[currentFrame+1];

            float timeDelta = (currentTime - startFrame.time) / (endFrame.time - startFrame.time);
            
        }
    }
    
    public void update(float tpf){
        currentTime += tpf;
        if (activeAnimation != null){
            
//            Keyframe[] frames = activeAnimation.frames;
//            int lastFrame = frames.length - 1;
//            if (lastFrame == currentFrame){
//                switch (getRepeatType()){
//                    case RT_CLAMP:
//                        
//                }
//            }
        }
    }
    
}
