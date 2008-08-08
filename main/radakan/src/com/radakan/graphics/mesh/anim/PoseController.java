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

import java.nio.FloatBuffer;
import java.util.Map;

import com.jme.scene.Controller;
import com.jme.scene.TriMesh;

/**
 * Non-functional class, do not use.
 */
public class PoseController extends Controller {

	private static final long serialVersionUID = 1301423379092925973L;
	private Map<String, Pose> poseMap;
    private FloatBuffer originalVertBuf;
    private TriMesh target;
    
    private Pose curPose = null;
    private float blendTime = 0f;
    private float blendTimeMax = 0f;
    
 
    
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
