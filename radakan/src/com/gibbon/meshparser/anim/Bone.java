package com.gibbon.meshparser.anim;

import com.jme.math.TransformMatrix;

public class Bone {
    
    String name;
    Bone parent;
    Bone[] children;
    
    /**
     * The local animated transform matrix
     */
    TransformMatrix animTransform = new TransformMatrix();
    
    /**
     * The final matrix used for skinning verticles applied to this bone.
     */
    TransformMatrix skinningTransform = new TransformMatrix();
    
    /**
     * The inverse bind matrix. Also known as the offset matrix.
     */
    TransformMatrix inverseBind = new TransformMatrix();
}
