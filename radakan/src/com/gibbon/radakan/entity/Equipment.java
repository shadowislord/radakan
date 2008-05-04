package com.gibbon.radakan.entity;

import java.util.*;
/*import com.jme.bounding.*;*/

import com.gibbon.radakan.entity.Entity.Type;

public final class Equipment {
	public enum Slot {
		HAT, BODYITEM, BACKPACK, LEFTARMITEM, RIGHTARMITEM, LEFTHANDITEM, RIGHTHANDITEM, LEFTLEGITEM, 
		RIGHTLEGITEM, LEFTSHOE, RIGHTSHOE
	}
	
	public Equipment (String ownersName) {
		replace(Slot.BACKPACK, new Entity(ownersName+"'s Backpack", Entity.Type.CONTAINER, "missing meshname", 3, null));
		/*replace(Slot.RIGHTHANDITEM, new Weapon ("Sword", Item.Type.PLAIN, 4, null));*/
	}
	
	public Entity get(Slot slot) {
		if (! slots.containsKey(slot))
		{
			slots.put(slot, null);
		}
		return slots.get(slot);
	}

	public Entity replace(Slot slot, Entity item) {
		if (! slots.containsKey(slot))
		{
			slots.put(slot, item);
			return null;
		}
		Entity result = slots.remove(slot);
		slots.put(slot, item);
		return result;
	}

	float getTotalMass() {
		float result = 0;
		for (Entity item : slots.values())
		{
			result += item.getTotalMass();
		}
		return result;
	}

	private Map<Slot,Entity> slots = new HashMap<Slot,Entity>();
};