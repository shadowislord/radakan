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

package com.gibbon.meshparser.anim;

import com.jme.math.Vector3f;
import com.jme.scene.TriMesh;
import com.jme.util.geom.BufferUtils;
import java.nio.FloatBuffer;

/**
 * A pose is just a list of offsets that say where a mesh verticles should be at the pose.
 */
public class Pose {

    private String name;
    private TriMesh target;

    private Vector3f[] offsets;
    private int[] indices;
    
    private Vector3f tempVec  = new Vector3f();
    private Vector3f tempVec2 = new Vector3f();
    
    public Pose(String name, TriMesh target){
        this.name = name;
        this.target = target;
    }
    
    public TriMesh getTarget(){
        return target;
    }
    
    public void setData(Vector3f[] offsets, int[] indices){
        this.offsets = offsets;
        this.indices = indices;
    }
    
    /**
     * Applies the offsets of this pose to the vertex buffer given by the blend factor.
     * 
     * @param blend Blend factor, 0 = no change to vert buf, 1 = apply full offsets
     * @param vertbuf Vertex buffer to apply this pose to
     */
    public void apply(float blend, FloatBuffer vertbuf){
        for (int i = 0; i < indices.length; i++){
            Vector3f offset = offsets[i];
            int vertIndex   = indices[i];
            
            tempVec.set(offset).multLocal(blend);
            
            // aquire vert
            BufferUtils.populateFromBuffer(tempVec2, vertbuf, vertIndex);
            
            // add offset multiplied by factor
            tempVec2.addLocal(tempVec);
            
            // write modified vert
            BufferUtils.setInBuffer(tempVec2, vertbuf, vertIndex);
        }
    }
    
}
