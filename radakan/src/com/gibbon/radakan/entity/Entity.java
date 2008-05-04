/*
 * Radakan RPG is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan RPG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan RPG.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.gibbon.radakan.entity;

/*import java.util.*;*/
import com.jme.bounding.*;

public final class Entity {
	public enum Type {
		CONTAINER, NPC, PLAIN, PLAYER_CHARACTER
	}
	
	public Entity (String newName, Type type, String newMeshName, float newMass, BoundingVolume newBoundingVolume) {
		this (newName, type, newMeshName, newMass, newBoundingVolume, "no specie");
	}

	public Entity (String newName, Type type, String newMeshName, float newMass, BoundingVolume newBoundingVolume, 
			String newSpecie) {
		name = newName;
		mass = newMass;
		boundingVolume = newBoundingVolume;
		
		if (type == Type.CONTAINER) {
			container = new ItemContainer ();
		} else {
			container = null;
		}
		
		if ((type == Type.NPC) || (type == Type.PLAYER_CHARACTER)) {
			character = new Character (this, type, newSpecie);
		} else {
			character = null;
		}
		
		meshName = newMeshName;
	}
	
	float getTotalMass() {
		float result = mass;
		if (container != null) {
			result += container.getTotalMass();
		}
		if (character != null) {
			if (character.equipment != null) {
				result += character.equipment.getTotalMass();
			}
		}
		return result;
	}
	
	public final String name;
	public final float mass;
	public final BoundingVolume boundingVolume;
	public final ItemContainer container;
	public final Character character;
	public final String meshName;
	public final boolean solid = true;;
}
