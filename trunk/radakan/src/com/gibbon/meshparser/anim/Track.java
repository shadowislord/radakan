package com.gibbon.meshparser.anim;

import com.jme.scene.TriMesh;

public abstract class Track {

    protected TriMesh target;
    
    public Track(TriMesh target){
        this.target = target;
    }
    
    public TriMesh getTarget(){
        return target;
    }
    
    public void setTarget(TriMesh target){
        this.target = target;
    }
    
    public abstract void setTime(float time);
    
}
