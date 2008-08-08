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

import com.jme.scene.TriMesh;
import com.jme.util.geom.BufferUtils;
import java.nio.FloatBuffer;

/**
 * Holds the bind pose and a weightbuffer that defines vertex->bone/weight associations.
 */
public class OgreMesh extends TriMesh {

    private transient FloatBuffer vertexBufferOriginal;
    private WeightBuffer weightBuffer;
    
    public OgreMesh(String name){
        super(name);
    }
    
    public void setWeightBuffer(WeightBuffer weightBuf){
        if (weightBuf.indexes.limit() != this.getVertexCount())
            throw new IllegalArgumentException();
        
        weightBuffer = weightBuf;
    }
    
    public WeightBuffer getWeightBuffer(){
        return weightBuffer;
    }
    
    public FloatBuffer getVertexBufferOriginal(){
        return vertexBufferOriginal;
    }
    
    public void clearBindPose(){
        vertexBufferOriginal = null;
    }
    
    public void saveCurrentToBindPose(){
        if (vertexBufferOriginal == null){
            vertexBufferOriginal = BufferUtils.createFloatBuffer(vertBuf.capacity());
        }
        
        vertBuf.rewind();
        vertexBufferOriginal.rewind();
        
        vertexBufferOriginal.put(vertBuf);
    }
    
    public void restoreBindPose(){
        vertBuf.rewind();
        vertexBufferOriginal.rewind();
        
        vertBuf.put(vertexBufferOriginal);
    }
    
    public boolean hasBindPose(){
        return vertexBufferOriginal != null;
    }
    
}
