package com.gibbon.meshparser;

import com.jme.math.Vector3f;
import com.jme.scene.TriMesh;
import com.jme.util.geom.BufferUtils;
import java.nio.FloatBuffer;

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
