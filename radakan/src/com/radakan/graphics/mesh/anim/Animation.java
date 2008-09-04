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

/**
 * Combines mesh and bone animations into one class for easier access
 */
public class Animation {

    private final String name;
    private float length;
    
    private BoneAnimation boneAnim;
    private MeshAnimation meshAnim;
    
    Animation(BoneAnimation boneAnim, MeshAnimation meshAnim){
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
    
    void setBoneAnimation(BoneAnimation boneAnim){
        this.boneAnim = boneAnim;
        
        this.length = Math.max(boneAnim.getLength(),
                               meshAnim.getLength());
    }
    
    void setMeshAnimation(MeshAnimation meshAnim){
        this.meshAnim = meshAnim;
        
        this.length = Math.max(boneAnim.getLength(),
                               meshAnim.getLength());
    }
    
    boolean hasMeshAnimation(){
        return meshAnim != null;
    }
    
    boolean hasBoneAnimation(){
        return boneAnim != null;
    }
    
    String getName(){
        return name;
    }
    
    float getLength(){
        return length;
    }
    
    void setTime(float time, OgreMesh[] targets, Skeleton skeleton){
        // we must apply the mesh animation first, 
        // as the vertex offsets are in the bind pose.
        if (meshAnim != null)
            meshAnim.setTime(time, targets);
        
        if (boneAnim != null){
            boneAnim.setTime(time, skeleton);
        }
    }

    MeshAnimation getMeshAnimation() {
        return meshAnim;
    }
    
    BoneAnimation getBoneAnimation(){
        return boneAnim;
    }
    
    
    
}
