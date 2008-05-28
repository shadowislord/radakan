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

package com.gibbon.meshparser.anim;

import com.jme.math.Matrix4f;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.scene.state.GLSLShaderObjectsState;
import java.util.List;

/**
 * A skeleton is a hierarchy of bones.
 * Skeleton updates the world transforms to reflect the current local
 * animated matrixes.
 */
public class Skeleton {

    private Bone rootBone;
    private Bone[] boneList;
    private Matrix4f[] skinningMatrixes;
    
    private final Quaternion tempQ = new Quaternion();
    private final Vector3f tempV = new Vector3f();
    
    public Skeleton(List<Bone> bones){
        for (Bone b : bones)
            if (b.parent == null)
                rootBone = b;
        
        boneList = bones.toArray(new Bone[0]);
        skinningMatrixes = new Matrix4f[bones.size()];
        for (int i = 0; i < skinningMatrixes.length; i++)
            skinningMatrixes[i] = new Matrix4f();
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
    
    public void sendToShader(GLSLShaderObjectsState shader){
        for (int i = 0; i < boneList.length; i++){
            Matrix4f matrix = skinningMatrixes[i];
            
            boneList[i].skinningTransform.getRotation(tempQ);
            boneList[i].skinningTransform.getTranslation(tempV);
            
            matrix.setTranslation(tempV);
            matrix.setRotationQuaternion(tempQ);
            
            boneList[i].skinningTransform.getScale(tempV);
            matrix.m00 *= tempV.x;
            matrix.m11 *= tempV.y;
            matrix.m22 *= tempV.z;
        }
        
        shader.setUniform("skinningMats", skinningMatrixes, false);
    }
    
}
