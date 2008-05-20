package com.gibbon.meshparser.anim;

import com.gibbon.meshparser.*;

public class MeshAnimation {

    private String name;
    private float length;
    private Track[] tracks;
    
    public MeshAnimation(String name, float length){
        this.name = name;
        this.length = length;
    }
    
    public String getName(){
        return name;
    }
    
    public float getLength(){
        return length;
    }
    
    public void setTracks(Track[] tracks){
        this.tracks = tracks;
    }
    
    public Track[] getTracks(){
        return tracks;
    }
    
    public void setTime(float time){
        for (int i = 0; i < tracks.length; i++){
            tracks[i].setTime(time);
        }
    }
    
}
