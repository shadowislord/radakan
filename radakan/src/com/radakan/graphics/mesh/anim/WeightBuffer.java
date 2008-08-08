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

    ByteBuffer indexes;
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
    
}
