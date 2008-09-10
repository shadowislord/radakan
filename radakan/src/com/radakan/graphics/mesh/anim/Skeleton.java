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
import com.jme.scene.state.GLSLShaderObjectsState;

/**
 * A skeleton is a hierarchy of bones.
 * Skeleton updates the world transforms to reflect the current local
 * animated matrixes.
 */
public final class Skeleton {

    private Bone rootBone;
    private Bone[] boneList;
    
    /**
     * Same as bone.skinningMatrix except as a Matrix4f object and not TransformMatrix
     * (easier to send to hardware)
     */
    private transient Matrix4f[] skinningMatrixes;
    
    public Skeleton(Bone[] boneList){
        this.boneList = boneList;
        for (Bone b : boneList){
            if (b.parent == null){
                if (rootBone != null)
                    throw new IllegalStateException("Cannot have more than one root bone in skeleton!");
                
                rootBone = b;
            }
        }
        
        createSkinningMatrices();
        
        rootBone.update();
        rootBone.setBindingPose();
    }
    
    public Skeleton(Skeleton source){
        Bone[] sourceList = source.boneList;
        boneList = new Bone[sourceList.length];
        for (int i = 0; i < sourceList.length; i++)
            boneList[i] = new Bone(sourceList[i]);
        
        rootBone = recreateBoneStructure(source.rootBone);
        
        createSkinningMatrices();
        
        rootBone.update();
    }
    
    private void createSkinningMatrices(){
        skinningMatrixes = new Matrix4f[boneList.length];
        for (int i = 0; i < skinningMatrixes.length; i++)
            skinningMatrixes[i] = new Matrix4f();
    }
    
    private Bone recreateBoneStructure(Bone sourceRoot){
        Bone targetRoot = getBone(sourceRoot.name);
        
        for (Bone sourceChild : sourceRoot.children){
            // find my version of the child
            Bone targetChild = getBone(sourceChild.name);
            targetRoot.addChild(targetChild);
            recreateBoneStructure(sourceChild);
        }
        
        return targetRoot;
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
    
    public int getBoneIndex(Bone bone){
        for (int i = 0; i < boneList.length; i++)
            if (boneList[i] == bone)
                return i;
        
        return -1;
    }
    
    public Matrix4f[] computeSkinningMatrices(){
        for (int i = 0; i < boneList.length; i++){
            boneList[i].getOffsetTransform(skinningMatrixes[i]);
        }
        return skinningMatrixes;
    }
    
    public int getBoneCount() {
        return boneList.length;
    }
    
    public void sendToShader(GLSLShaderObjectsState shader){
        Matrix4f[] skinningMats = computeSkinningMatrices();
        //shader.setUniform("boneMatrices", skinningMats, true);
    }
 
}
