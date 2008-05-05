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

import java.util.*;


public class ItemContainer implements Set <Entity2> {
	
	public boolean add(Entity2 o) {
		return items.add(o);
	}
	public boolean addAll(Collection c) {
		return items.addAll(c);
    }
	public void clear() {
		items.clear();
	}
	public boolean contains(Object o) {
		return items.contains(o);	
	}
	public boolean containsAll(Collection c) {
		return items.containsAll(c);	
	}
	public boolean equals(Object o) {
		return items.equals(o);
	}
	public int hashCode() {
		return items.hashCode();
	}
	public boolean isEmpty() {
		return items.isEmpty();
	}
	public Iterator<Entity2> iterator() {
		return items.iterator();
	}
	public boolean remove(Object o) {
		return items.remove(o);
	}
	public boolean removeAll(Collection collection) {
		return items.removeAll(collection);
	}
	public boolean retainAll(Collection collection) {
		return items.retainAll(collection);
	}
	public int size() {
		return items.size();
	}
	public Object[] toArray() {
		return items.toArray();
	}
	public Object[] toArray(Object[] a) {
		return items.toArray(a);
	}
	
	public float getTotalMass() {
		float result = 0;
		for (Entity2 item : items)
		{
			result += item.getTotalMass();
		}
		return result;
	}
	
	private HashSet <Entity2> items;
}
