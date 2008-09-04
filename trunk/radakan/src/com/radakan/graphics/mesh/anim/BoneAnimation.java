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

public final class BoneAnimation {

    private final String name;
    private final float length;
    
    private final BoneTrack[] tracks;
    
    BoneAnimation(String name, float length, BoneTrack[] tracks){
        this.name = name;
        this.length = length;
        this.tracks = tracks;
    }
    
    String getName(){
        return name;
    }
    
    float getLength(){
        return length;
    }
    
    BoneTrack[] getTracks(){
        return tracks;
    }
    
    void setTime(float time, Skeleton skeleton){
        for (int i = 0; i < tracks.length; i++){
            tracks[i].setTime(time, skeleton);
        }
    }
    
}
