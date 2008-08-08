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

public class MeshAnimation {

    private String name;
    private float length;
    private Track[] tracks;
    
    public MeshAnimation(String name, float length){
        this.name = name;
        this.length = length;
    }
    
    public String getName(){
        return name;
    }
    
    public float getLength(){
        return length;
    }
    
    public void setTracks(Track[] tracks){
        this.tracks = tracks;
    }
    
    public Track[] getTracks(){
        return tracks;
    }
    
    public void setTime(float time){
        for (int i = 0; i < tracks.length; i++){
            tracks[i].setTime(time);
        }
    }
    
}
