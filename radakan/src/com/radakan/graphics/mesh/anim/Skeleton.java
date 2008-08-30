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
import com.jme.scene.state.GLSLShaderObjectsState;
import com.jme.util.resource.SimpleResourceLocator;
import java.util.List;

/**
 * A skeleton is a hierarchy of bones.
 * Skeleton updates the world transforms to reflect the current local
 * animated matrixes.
 */
public class Skeleton {

    private Bone rootBone;
    private Bone[] boneList;
    
    /**
     * Same as bone.skinningMatrix except as a Matrix4f object and not TransformMatrix
     * (easier to send to hardware)
     */
    private Matrix4f[] skinningMatrixes;
    
    private final Quaternion tempQ = new Quaternion();
    private final Vector3f tempV = new Vector3f();
    
    public Skeleton(Bone[] boneList){
        this.boneList = boneList;
        
        for (Bone b : boneList){
            if (b.parent == null){
                if (rootBone != null)
                    throw new IllegalStateException("Cannot have more than one root bone in skeleton!");
                
                rootBone = b;
            }
        }
        
        skinningMatrixes = new Matrix4f[boneList.length];
        for (int i = 0; i < skinningMatrixes.length; i++)
            skinningMatrixes[i] = new Matrix4f();
        
        rootBone.setBindingPose();
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
    
    public void resetAnimationTransforms(){
        rootBone.reset();
    }
    
    public void sendToShader(GLSLShaderObjectsState shader){
//        for (int i = 0; i < boneList.length; i++){
//            Matrix4f matrix = skinningMatrixes[i];
//            
//            boneList[i].skinningTransform.getRotation(tempQ);
//            boneList[i].skinningTransform.getTranslation(tempV);
//            
//            matrix.setTranslation(tempV);
//            matrix.setRotationQuaternion(tempQ);
//            
//            boneList[i].skinningTransform.getScale(tempV);
//            matrix.m00 *= tempV.x;
//            matrix.m11 *= tempV.y;
//            matrix.m22 *= tempV.z;
//        }
        
        // FIXME: Apply jME uniform patch
        //shader.setUniform("skinningMats", skinningMatrixes, false);
    }
    
}
