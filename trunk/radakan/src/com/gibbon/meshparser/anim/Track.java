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

import com.jme.scene.TriMesh;

/**
 * A single track of mesh animation (either morph or pose based).
 * Currently morph animations are not supported (only pose).
 */
public abstract class Track {

    protected TriMesh target;
    
    public Track(TriMesh target){
        this.target = target;
    }
    
    public TriMesh getTarget(){
        return target;
    }
    
    public void setTarget(TriMesh target){
        this.target = target;
    }
    
    public abstract void setTime(float time);
    
}
