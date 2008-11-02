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

package com.radakan.entity.unit;

import com.jme.math.Vector3f;

/**
 * PhysicsUnit concerns itself with making the Entity a 
 * physical object which can interact with other entities having a PhysicsUnit.
 * 
 * The PhysicsUnit makes sure the player stays on terrain, 
 * and keeps from colliding with the terrain and other objects, 
 * applying the forces that are specified to it.
 * 
 * @author Joshua Montgomery
 * @author Kirill Vainer
 * @created Aug 23, 2008
 */
public interface IPhysicsUnit extends IUnit {
    
    /**
     * Adds force to the Entity's current force.
     * Generally makes an entity move toward the given vector by it's magnitude, 
     * if friction or collision does not stop it.
     * 
     * @param force The force vector to be added.
     */
    public void addForce(Vector3f force);
    
    /**
     * Same as addForce(), but instead replaces the current force with the 
     * provided vector.
     * 
     * @param force The force vector to be set.
     */
    public void setForce(Vector3f force);
    
    /**
     * @return the current force applied to the entity.
     */
    public Vector3f getForce();
}
