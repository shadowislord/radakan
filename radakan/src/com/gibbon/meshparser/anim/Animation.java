package com.gibbon.meshparser.anim;

public class Animation {

    private String name;
    private float length;
    
    private BoneAnimation boneAnim;
    private MeshAnimation meshAnim;
    
    public Animation(BoneAnimation boneAnim, MeshAnimation meshAnim){
        this.boneAnim = boneAnim;
        this.meshAnim = meshAnim;
        
        if (boneAnim == null){
            this.name = meshAnim.getName();
            this.length = meshAnim.getLength();
        }else if (meshAnim == null){
            this.name = boneAnim.getName();
            this.length = boneAnim.getLength();
        }else{
            this.name = boneAnim.getName();
            this.length = Math.max(boneAnim.getLength(),
                                   meshAnim.getLength());
        }
    }
    
    public String getName(){
        return name;
    }
    
    public float getLength(){
        return length;
    }
    
    public void setTime(float time){
        // we must apply the mesh animation first, 
        // as the vertex offsets are in the bind pose.
        if (meshAnim != null)
            meshAnim.setTime(time);
        
        if (boneAnim != null){
            boneAnim.setTime(time);
        }
    }
    
    
    
}
