package com.gibbon.meshparser.anim;

public class BoneAnimation {

    private String name;
    private float length;
    
    private BoneTrack[] tracks;
    private Skeleton skeleton;
    
    public BoneAnimation(String name, float length){
        this.name = name;
        this.length = length;
    }
    
    public String getName(){
        return name;
    }
    
    public float getLength(){
        return length;
    }
    
    public void setData(Skeleton skeleton, BoneTrack[] tracks){
        this.tracks = tracks;
        this.skeleton = skeleton;
    }
    
    public BoneTrack[] getTracks(){
        return tracks;
    }
    
    public void setTime(float time){
        for (int i = 0; i < tracks.length; i++){
            tracks[i].setTime(time);
        }
        
        skeleton.updateWorldTransforms();
    }
    
}
