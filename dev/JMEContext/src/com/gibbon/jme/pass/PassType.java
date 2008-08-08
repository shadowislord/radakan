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

package com.gibbon.jme.pass;

/**
 * Defines pass execution buckets
 * 
 * @author Momoko_Fan
 */
public enum PassType {

    /**
     * Polling input
     */
    INPUT (0),
    
    /**
     * Accepting input and updating neccessary objects
     */
    POST_INPUT (1),
    
    /**
     * Updating scene based on input and state changes
     */
    UPDATE (2),
    
    /**
     * Pre rendering (background)
     */
    PRE_RENDER (3),
    
    /**
     * Rendering the scene to the framebuffer
     */
    RENDER (4),
    
    /**
     * Doing post-render handling (GUI)
     */
    POST_RENDER (5);
            
    final int priority;
    
    PassType(int priority){
        this.priority = priority;
    }
    
}
