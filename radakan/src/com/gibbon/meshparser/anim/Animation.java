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

/**
 * Combines mesh and bone animations into one class for easier access
 */
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
    
    public boolean hasMeshAnimation(){
        return meshAnim != null;
    }
    
    public boolean hasBoneAnimation(){
        return boneAnim != null;
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

    public MeshAnimation getMeshAnimation() {
        return meshAnim;
    }
    
    public BoneAnimation getBoneAnimation(){
        return boneAnim;
    }
    
    
    
}
