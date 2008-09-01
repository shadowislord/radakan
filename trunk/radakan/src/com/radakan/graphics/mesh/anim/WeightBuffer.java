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

import com.jme.scene.state.GLSLShaderObjectsState;
import com.jme.util.geom.BufferUtils;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;

/**
 * WeightBuffer contains associations of vertexes to bones and their weights.
 * The WeightBuffer can be sent to a shader or processed on the CPU 
 * to do skinning.
 */
public class WeightBuffer {

    /**
     * Each 4 bytes in the boneIndex buffer are assigned to a vertex.
     * 
     */
    ByteBuffer indexes;
    
    /**
     * The weight of each bone specified in the index buffer
     */
    FloatBuffer weights;
    
    public WeightBuffer(int vertexCount){
        indexes = BufferUtils.createByteBuffer(vertexCount * 4);
        weights = BufferUtils.createFloatBuffer(vertexCount * 4);
    }
    
    public void sendToShader(GLSLShaderObjectsState shader){
        //shader.setAttributePointer("indexCount", 1, false, true, 0, indexCounts);
        shader.setAttributePointer("indexes", 4, false, true, 0, indexes);
        shader.setAttributePointer("weights", 4, true, 0, weights);
    }
    
    public void normalizeWeights(){
        int nVerts = weights.capacity() / 4;
        weights.rewind();
        for (int v = 0; v < nVerts; v++){
            float w0 = weights.get(),
                  w1 = weights.get(),
                  w2 = weights.get(),
                  w3 = weights.get();
            float sum = w0 + w1 + w2 + w3;
            if (sum != 1f){
                weights.position(weights.position()-4);
                weights.put(w0 / sum);
                weights.put(w1 / sum);
                weights.put(w2 / sum);
                weights.put(w3 / sum);
            }
        }
        weights.rewind();
    }
    
}
