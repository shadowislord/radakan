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
import com.jme.math.Matrix4f;
import com.jme.math.Quaternion;
import com.jme.math.TransformMatrix;
import com.jme.math.Vector3f;
import java.util.ArrayList;

public class Bone {
    
    private static final int mode = 2;
    
    public String name;
    public Bone parent;
    public ArrayList<Bone> children = new ArrayList<Bone>();
    
    public Vector3f tempV = new Vector3f();
    public Quaternion tempQ = new Quaternion();
    
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
    
    public void setInitialState(){
        initialPos.set(localPos);
        initialRot.set(localRot);
    }
    
    public void resetToInitialState(){
        localPos.set(initialPos);
        localRot.set(initialRot);
    }
    
    public void setBindingPose(){
        setInitialState();
        
        updateFromParent();

        // Save inverse derived position/scale/orientation, used for calculate offset transform later
        worldBindInversePos.set(worldPos);
        worldBindInversePos.negateLocal();
        
        worldBindInverseRot.set(worldRot);
        worldBindInverseRot.inverseLocal();
        
        for (Bone b : children)
            b.setBindingPose();
    }
    
    public void reset(){
        resetToInitialState();
        
        for (Bone b : children)
            b.reset();
    }
    
    public void getOffsetTransform(Matrix4f m){
        Quaternion rotate = worldRot.mult(worldBindInverseRot);
        Vector3f translate = worldPos.add(rotate.mult(worldBindInversePos));
        
        m.loadIdentity();
        m.setTranslation(translate);
        m.setRotationQuaternion(rotate);
    }
    
    public void setAnimTransforms(Vector3f translation, Quaternion rotation, Vector3f scale){
        // TRANSLATE
        if (mode == 1){
            tempV.set(translation);
            localRot.multLocal(tempV);
            localPos.addLocal(tempV);
        }else if (mode == 2){
            if (parent != null){
                tempQ.set(parent.worldRot).inverseLocal();
                tempQ.mult(translation, tempV);
                localPos.addLocal(tempV);
            }else{
                localPos.addLocal(translation);
            }
        }else if (mode == 3){
            localPos.addLocal(translation);
        }
        
        // ROTATE
        if (mode == 1){
            rotation.mult(localRot, localRot);
        }else if (mode == 2){
            tempQ.set(localRot).multLocal(worldRot.inverse()).multLocal(rotation).multLocal(worldRot);
            localRot.set(tempQ);
        }else if (mode == 3){
            localRot.multLocal(rotation);
        }
        
//        animMat.setTranslation(translation);
//        animMat.setRotationQuaternion(rotation);
//        animMat.setScale(scale);
    }
    
    public void setBindTransforms(Vector3f translation, Quaternion rotation, Vector3f scale){
        initialPos.set(translation);
        initialRot.set(rotation);
        
        localPos.set(translation);
        localRot.set(rotation);
    }
    
    public void setAnimTransforms(Vector3f translation, Quaternion rotation){
        setAnimTransforms(translation, rotation, Vector3f.UNIT_XYZ);
    }
    
    public void setBindTransforms(Vector3f translation, Quaternion rotation){
        setBindTransforms(translation, rotation, Vector3f.UNIT_XYZ);
    }
    
    public void updateFromParent(){
        if (parent != null){
            worldRot.set(localRot).multLocal(parent.worldRot);
            
            parent.worldRot.mult(localPos, worldPos);
            worldPos.addLocal(parent.worldPos);
        }else{
            worldRot.set(localRot);
            worldPos.set(localPos);
        }
    }
    
    public void update(){
        updateFromParent();
        
        for (Bone b : children)
            b.update();
    }
    

    private Vector3f initialPos = new Vector3f();
    private Quaternion initialRot = new Quaternion();
    
    /**
     * (BONE SPACE)
     * The local animated transform matrix. 
     */
    private Vector3f localPos = new Vector3f();
    private Quaternion localRot = new Quaternion();
    
    /**
     * (MODEL SPACE)
     */
    private Vector3f worldPos = new Vector3f();
    private Quaternion worldRot = new Quaternion();
    
    /**
     * (MODEL SPACE)
     * The inverse bind matrix. 
     * Convert MODEL space to BONE space
     */
    private Vector3f worldBindInversePos = new Vector3f();
    private Quaternion worldBindInverseRot = new Quaternion();
    
}
