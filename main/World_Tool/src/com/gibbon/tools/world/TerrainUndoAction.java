package com.gibbon.tools.world;

import com.jme.scene.TriMesh;
import com.jme.util.geom.BufferUtils;
import java.nio.FloatBuffer;

public class TerrainUndoAction implements UndoAction {

    private String name;
    private FloatBuffer[] stateVertex;
    private FloatBuffer[] stateNormal;
    private TriMesh[] influenced;
    
    public FloatBuffer clone(FloatBuffer data){
        FloatBuffer copy = BufferUtils.createFloatBuffer(data.capacity());
        data.rewind();
        copy.put(data);
        copy.rewind();
        return copy;
    }
    
    public TerrainUndoAction(String name, TriMesh[] meshes){
        this.name = name;
        
        influenced = meshes;
        stateVertex = new FloatBuffer[meshes.length];
        stateNormal = new FloatBuffer[meshes.length];
        for (int i = 0; i < meshes.length; i++){
            stateVertex[i] = clone(meshes[i].getVertexBuffer());
            stateNormal[i] = clone(meshes[i].getNormalBuffer());
        }
    }
    
    public UndoAction restore(boolean returnRedoAction) {
        TerrainUndoAction redo = null;
        if (returnRedoAction){
            // saves the current state before restoring it
            // so you can redo the undo
            redo = new TerrainUndoAction("Redo '"+name+"'", influenced);
        }
        
        for (int i = 0; i < influenced.length; i++){
            influenced[i].setVertexBuffer(stateVertex[i]);
            influenced[i].setNormalBuffer(stateNormal[i]);
        }
        
        return redo;
    }

    public String getName() {
        return name;
    }

}
