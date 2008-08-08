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
     * 
     * This will probably be removed when
     * Skeleton.updateWorldTransforms()
     * and 
     * Skeleton.sendToShader()
     * are condenced to one method.
     */
    TransformMatrix skinningTransform = new TransformMatrix();
    
    /**
     * The inverse bind matrix. Also known as the offset matrix.
     */
    TransformMatrix inverseBind = new TransformMatrix();
}
