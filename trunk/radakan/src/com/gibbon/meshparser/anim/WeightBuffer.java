package com.gibbon.meshparser.anim;

import com.jme.scene.state.GLSLShaderObjectsState;
import com.jme.util.geom.BufferUtils;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;

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
