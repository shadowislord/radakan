package com.gibbon.tools.world;

import com.jme.bounding.BoundingBox;
import com.jme.scene.Node;
import com.jme.scene.TriMesh;

public class Tile extends Node {

    private TriMesh terrain;
    private int x, y;
    
    public Tile(int x, int y){
        super("TILE_"+x+"_"+y);
        this.x = x;
        this.y = y;
        setModelBound(new BoundingBox());
    }
    
    public int getX(){
        return x;
    }
    
    public int getY(){
        return y;
    }
    
    public void setTerrain(TriMesh terrain){
        this.terrain = terrain;
        attachChild(terrain);
        updateModelBound();
        updateWorldBound();
    }
    
    public TriMesh getTerrain(){
        return terrain;
    }
    
}
