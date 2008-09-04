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

import com.jme.math.Vector3f;
import com.jme.scene.TriMesh;
import com.jme.util.geom.BufferUtils;
import java.nio.FloatBuffer;

/**
 * A pose is just a list of offsets that say where a mesh verticles should be at the pose.
 */
public final class Pose {

    private final String name;
    private final int targetMeshIndex;

    private final Vector3f[] offsets;
    private final int[] indices;
    
    private final Vector3f tempVec  = new Vector3f();
    private final Vector3f tempVec2 = new Vector3f();
    
    public Pose(String name, int targetMeshIndex, Vector3f[] offsets, int[] indices){
        this.name = name;
        this.targetMeshIndex = targetMeshIndex;
        this.offsets = offsets;
        this.indices = indices;
    }
    
    public int getTargetMeshIndex(){
        return targetMeshIndex;
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
