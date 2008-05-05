package com.gibbon.radakan.entity;

import java.util.*;
/*import com.jme.bounding.*;*/

import com.gibbon.radakan.entity.Entity2.Type;

public final class Equipment {
    
	public enum Slot {
		HAT, BODYITEM, BACKPACK, LEFTARMITEM, RIGHTARMITEM, LEFTHANDITEM, RIGHTHANDITEM, LEFTLEGITEM, 
		RIGHTLEGITEM, LEFTSHOE, RIGHTSHOE
	}
	
	public Equipment (String ownersName) {
		replace(Slot.BACKPACK, new Entity2(ownersName+"'s Backpack", "backpack"));
		/*replace(Slot.RIGHTHANDITEM, new Weapon ("Sword", Item.Type.PLAIN, 4, null));*/
	}
	
	public Entity2 get(Slot slot) {
		if (! slots.containsKey(slot))
		{
			slots.put(slot, null);
		}
		return slots.get(slot);
	}

	public Entity2 replace(Slot slot, Entity2 item) {
		if (! slots.containsKey(slot))
		{
			slots.put(slot, item);
			return null;
		}
		Entity2 result = slots.remove(slot);
		slots.put(slot, item);
		return result;
	}

	float getTotalMass() {
		float result = 0;
		for (Entity2 item : slots.values())
		{
			result += item.getTotalMass();
		}
		return result;
	}

	private Map<Slot,Entity2> slots = new HashMap<Slot,Entity2>();
};