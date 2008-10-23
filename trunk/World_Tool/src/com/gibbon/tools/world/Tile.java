package com.gibbon.tools.world;

import com.jme.bounding.BoundingBox;
import com.jme.scene.Node;
import com.jme.scene.TriMesh;
import com.jme.util.export.InputCapsule;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.jme.util.export.OutputCapsule;
import java.io.IOException;

public class Tile extends Node {

    private TriMesh terrain;
    private int x, y;
    private transient boolean modified = false;
    
    public Tile(int x, int y){
        super("tile_"+x+"_"+y);
        this.x = x;
        this.y = y;
        setModelBound(new BoundingBox());
    }
    
    public Tile(){
    }
    
    public void setModified(){
        modified = true;
    }
    
    public void clearModified(){
        modified = false;
    }
    
    public boolean isModified(){
        return modified;
    }
    
    public int getX(){
        return x;
    }
    
    public int getY(){
        return y;
    }
    
    public void setTerrain(TriMesh terrain){
        this.terrain = terrain;
        attachChildAt(terrain, 0);
        
        TileGroup group = getGroup();
        terrain.setRenderState(group.getTerrainShader());
        terrain.setRenderState(group.getTerrainTextureState());
        
        terrain.updateRenderState();
        updateModelBound();
        updateWorldBound();
    }
    
    public TriMesh getTerrain(){
        return terrain;
    }
    
    public TileGroup getGroup(){
        return (TileGroup) parent;
    }
    
    @Override
    public void read(JMEImporter im) throws IOException {
        super.read(im);
        
        InputCapsule cap = im.getCapsule(this);
        x = cap.readInt("TileX", 0);
        y = cap.readInt("TileY", 0);
        
        setName("tile_"+x+"_"+y);
        
        terrain = (TriMesh) getChild(0);
        updateModelBound();
        updateWorldBound();
    }
    
    @Override
    public void write(JMEExporter ex) throws IOException {
        super.write(ex);
        
        OutputCapsule cap = ex.getCapsule(this);
        cap.write(x, "TileX", 0);
        cap.write(y, "TileY", 0);
    }
    
}
