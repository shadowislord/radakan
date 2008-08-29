/*
 * Radakan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.radakan.graphics.mesh.anim;

import com.jme.math.Matrix3f;
import com.jme.math.Quaternion;
import com.jme.math.TransformMatrix;
import com.jme.math.Vector3f;
import java.util.ArrayList;

public class Bone {
    
    public String name;
    public Bone parent;
    public ArrayList<Bone> children = new ArrayList<Bone>();
    
    private Vector3f tempV = new Vector3f();
    
    public Bone(String name){
        this.name = name;
    }
    
    public Bone(String name, Bone parent){
        this(name);
        this.parent = parent;
    }

    public void addChild(Bone bone) {
        children.add(bone);
        bone.parent = this;
    }
    
    public void update(){
        //
        worldMat.set(bindMat);
        //worldMat.set(animMat);
        //worldMat.multLocal(bindMat, tempV);
        
        if (parent != null){
            //worldMat.combineWithParent(parent.worldMat);
        }
        
        worldMat.loadIdentity();
        
        for (Bone b : children)
            b.update();
    }
    
    public void computeInverseWorldBind(){
        worldMat.set(bindMat);
        if (parent != null){
            //worldMat.combineWithParent(parent.worldMat);
        }
        
        worldMat.loadIdentity();
        
        // make sure worldMat is in bind space
        worldBindInverseMat.set(worldMat);
        worldBindInverseMat.inverse();
        
        for (Bone b : children)
            b.computeInverseWorldBind();
    }
    
    public void getSkinningMatrix(TransformMatrix store){
        // SCALE
        Vector3f temp = new Vector3f(),
                 temp2 = new Vector3f();
        worldMat.getScale(temp);
        worldBindInverseMat.getScale(temp2);
        
        Vector3f scale = temp.mult(temp2);
        
        
        // ROT
        Quaternion tempQ = new Quaternion(),
                   tempQ2 = new Quaternion();
        worldMat.getRotation(tempQ);
        worldBindInverseMat.getRotation(tempQ2);
        
        Quaternion rotate = tempQ.mult(tempQ2);
                
        // TRANSLATE
        worldBindInverseMat.getTranslation(temp);
        worldMat.getTranslation(temp2);
        
        Vector3f translate = new Vector3f(scale);
        translate.multLocal(temp);
        rotate.multLocal(translate);
        translate.addLocal(temp2);

        store.setTranslation(translate);
        store.setRotationQuaternion(rotate);
        store.setScale(scale);
    }
    
    /**
     * (BONE SPACE)
     * The local animated transform matrix. 
     */
    public TransformMatrix animMat = new TransformMatrix();
    
    /**
     * (MODEL SPACE)
     */
    public TransformMatrix worldMat = new TransformMatrix();
    
    /**
     * (MODEL SPACE)
     * The inverse bind matrix. 
     * Convert MODEL space to BONE space
     */
    public TransformMatrix worldBindInverseMat = new TransformMatrix();
    
    /**
     * (PARENT SPACE)
     * The bind matrix. 
     * Convert PARENT space to BONE space
     */
    public TransformMatrix bindMat = new TransformMatrix();
}
