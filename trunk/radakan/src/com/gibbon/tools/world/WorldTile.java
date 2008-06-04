package com.gibbon.tools.world;

import com.jme.bounding.BoundingBox;
import com.jme.scene.Node;
import com.jme.scene.TriMesh;

public class WorldTile extends Node {

    private TriMesh terrain;
    
    public WorldTile(int x, int y){
        super("tile_"+x+"_"+y);
        setModelBound(new BoundingBox());
    }
    
    public void setTerrain(TriMesh terrain){
        this.terrain = terrain;
    }
    
    public TriMesh getTerrain(){
        return terrain;
    }
    
}
