package com.gibbon.tools.world;

import com.jme.scene.TriMesh;
import com.jme.util.geom.BufferUtils;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.List;

public class TerrainUndoAction implements UndoAction {

    private String name;
    private List<FloatBuffer> stateVertex = new ArrayList<FloatBuffer>();
    //private List<FloatBuffer> stateNormal = new ArrayList<FloatBuffer>();
    private List<TriMesh> influenced = new ArrayList<TriMesh>();
    
    public FloatBuffer clone(FloatBuffer data){
        FloatBuffer copy = BufferUtils.createFloatBuffer(data.capacity());
        data.rewind();
        copy.put(data);
        copy.rewind();
        return copy;
    }
    
    public TerrainUndoAction(String name){
        this.name = name;
    }

    public void addModifiedMesh(TriMesh mesh){
        // do not add same mesh twice
        if (influenced.contains(mesh))
            return;
        
        influenced.add(mesh);
        stateVertex.add(clone(mesh.getVertexBuffer()));
        //stateNormal.add(clone(mesh.getNormalBuffer()));
    }
    
    @Override
    public UndoAction restore(boolean returnRedoAction) {
        TerrainUndoAction redo = null;
        if (returnRedoAction){
            // saves the current state before restoring it
            // so you can redo the undo
            redo = new TerrainUndoAction("Redo '"+name+"'");
            for (TriMesh mesh : influenced)
                redo.addModifiedMesh(mesh);
        }
        
        for (int i = 0; i < influenced.size(); i++){
            TriMesh mesh = influenced.get(i);

            mesh.setVertexBuffer(stateVertex.get(i));
            //influenced.get(i).setNormalBuffer(stateNormal.get(i));

            Tile t = (Tile) mesh.getParent();
            //if (t.isModified()){
                TerrainUtil.rebuildNormalArray(t);
            //}
        }

        return redo;
    }

    @Override
    public String getName() {
        return name;
    }

}
