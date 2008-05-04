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

import com.gibbon.radakan.ai.ArtificialIntelligence;
import com.gibbon.radakan.ai.Behavior;
import com.gibbon.radakan.ai.Nothing;
import com.gibbon.radakan.ai.Player;
import com.gibbon.radakan.ai.Skill;
import com.gibbon.radakan.ai.StrategyStateMachine;
import java.util.*;

///	All characters observe each other.
public final class Character  {
	public class Type {
	    public String specie;
	    public boolean playerCharacter;
	    public boolean male;
	}
	
	public Character(Entity newEntity, Type type) {
		entity = newEntity;

		Behavior behavior = null;
		if(type.playerCharacter) {
			behavior = new Player(entity);
		} else {
			behavior = new ArtificialIntelligence(entity);
		}
		stateMachine = new StrategyStateMachine <Nothing, Behavior> (behavior);
		
		skills = new HashMap<String, Skill>();
		equipment = new Equipment(entity.name);
		
		specie = type.specie;
		male = type.male;
	}

	public void call(Nothing message) {
		stateMachine.run(Nothing.get());
	}

	public Boolean isAlive(){
		return stateMachine.hasState();
	}

	public final Entity entity;
	public final StrategyStateMachine <Nothing, Behavior> stateMachine;
	public final Map<String, Skill> skills;
	public final Equipment equipment;
	public final String specie;
	public final boolean male;
};
