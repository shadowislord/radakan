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

package com.gibbon.radakan.entity;

import com.jme.bounding.*;
import java.util.HashMap;
import java.util.Map;

public final class Entity2 {

   public static class Type {

        public String type;
        public String meshName;
        public boolean solid;
        public boolean container;
        public Character.Type characterType;
        public boolean visible;
        public float mass;
        public BoundingVolume boundingVolume;
        public static Map<String, Type> map = new HashMap<String, Type>();
    }

	public Entity2 (String newName, String typeName) {
		Type type = Type.map.get(typeName);
		name = newName;
		mass = type.mass;
		boundingVolume = type.boundingVolume;
		
		if (type.container) {
			container = new ItemContainer ();
		} else {
			container = null;
		}
		
		if (type.characterType == null) {
			character = null;
		} else {
			character = new Character (this, type.characterType);
		}
		
		meshName = type.meshName;
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
