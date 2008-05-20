package com.gibbon.meshparser.anim;

import java.util.Arrays;
import java.util.List;

public class Skeleton {

    private Bone rootBone;
    private Bone[] boneList;
    
    public Skeleton(List<Bone> bones){
        for (Bone b : bones)
            if (b.parent == null)
                rootBone = b;
        
        boneList = bones.toArray(new Bone[0]);
    }
    
    public Bone getRoot(){
        return rootBone;
    }
    
    public Bone getBone(int index){
        return boneList[index];
    }
    
    public Bone getBone(String name){
        for (int i = 0; i < boneList.length; i++)
            if (boneList[i].name.equals(name))
                return boneList[i];
        
        return null;
    }
    
    private void updateWorldTransforms(Bone b){
        if (b.parent != null){
            b.skinningTransform.set(b.animTransform);
            b.skinningTransform.combineWithParent(b.parent.skinningTransform);
        }
        
        for (Bone c : b.children)
            updateWorldTransforms(c);
    }
    
    public void updateWorldTransforms(){
        updateWorldTransforms(rootBone);
    }
    
}
