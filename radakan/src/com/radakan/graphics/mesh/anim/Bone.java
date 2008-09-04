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

import com.jme.math.Matrix4f;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import java.util.ArrayList;

public final class Bone {
    
    final String name;
    
    Bone parent;
    final ArrayList<Bone> children = new ArrayList<Bone>();
    
    boolean userControl = false;
    
    private Vector3f initialPos;
    private Quaternion initialRot;
    
    /**
     * (MODEL SPACE)
     * The inverse bind matrix. 
     * Convert MODEL space to BONE space
     */
    private Vector3f worldBindInversePos;
    private Quaternion worldBindInverseRot;
    
    /**
     * (BONE SPACE)
     * The local animated transform matrix combined with the bind matrix
     */
    private Vector3f localPos = new Vector3f();
    private Quaternion localRot = new Quaternion();
    
    /**
     * (MODEL SPACE)
     */
    private Vector3f worldPos = new Vector3f();
    private Quaternion worldRot = new Quaternion();
    
    Bone(String name){
        this.name = name;
        
        initialPos = new Vector3f();
        initialRot = new Quaternion();
        
        worldBindInversePos = new Vector3f();
        worldBindInverseRot = new Quaternion();
    }
    
    Bone(Bone source){
        this(source.name);
        
        userControl = source.userControl;
        
        initialPos = source.initialPos.clone();
        initialRot = source.initialRot.clone();
        
        worldBindInversePos = source.worldBindInversePos.clone();
        worldBindInverseRot = source.worldBindInverseRot.clone();
        
        // parent and children will be assigned manually..
    }
    
    public void setUserControl(boolean enable){
        userControl = enable;
    }
    
    void addChild(Bone bone) {
        children.add(bone);
        bone.parent = this;
    }
    
    void updateWorldVectors(){
        if (parent != null){
            // worldRot = localRot * parentWorldRot
            worldRot = parent.worldRot.mult(localRot);
            
            // worldPos = parentWorldPos + (parentWorldRot * localPos)
            worldPos = parent.worldRot.mult(localPos).add(parent.worldPos);
        }else{
            worldRot.set(localRot);
            worldPos.set(localPos);
        }
    }
    
    void update(){
        updateWorldVectors();
        
        for (Bone b : children)
            b.update();
    }
    
    void setBindingPose(){
        initialPos.set(localPos);
        initialRot.set(localRot);
        
        // Save inverse derived position/scale/orientation, used for calculate offset transform later
        worldBindInversePos.set(worldPos);
        worldBindInversePos.negateLocal();
        
        worldBindInverseRot.set(worldRot);
        worldBindInverseRot.inverseLocal();
        
        for (Bone b : children)
            b.setBindingPose();
    }
    
    void reset(){
        if (!userControl){
            localPos.set(initialPos);
            localRot.set(initialRot);
        }
        
        for (Bone b : children)
            b.reset();
    }
    
    void getOffsetTransform(Matrix4f m){
        Quaternion rotate = worldRot.mult(worldBindInverseRot);
        Vector3f translate = worldPos.add(rotate.mult(worldBindInversePos));
        
        m.loadIdentity();
        m.setTranslation(translate);
        m.setRotationQuaternion(rotate);
    }
    
    public void setUserTransforms(Vector3f translation, Quaternion rotation, Vector3f scale){
        localPos.set(initialPos);
        localRot.set(initialRot);
        localPos.addLocal(translation);
        localRot = localRot.mult(rotation);
    }
    
    void setAnimTransforms(Vector3f translation, Quaternion rotation, Vector3f scale){
        if (userControl)
            return;
        
        localPos.addLocal(translation);
        localRot = localRot.mult(rotation);
    }
    
    void setBindTransforms(Vector3f translation, Quaternion rotation, Vector3f scale){
        initialPos.set(translation);
        initialRot.set(rotation);
        
        localPos.set(translation);
        localRot.set(rotation);
    }
    
    void setAnimTransforms(Vector3f translation, Quaternion rotation){
        setAnimTransforms(translation, rotation, Vector3f.UNIT_XYZ);
    }
    
    void setBindTransforms(Vector3f translation, Quaternion rotation){
        setBindTransforms(translation, rotation, Vector3f.UNIT_XYZ);
    }
    
     
}
