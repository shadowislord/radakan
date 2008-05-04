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

import java.util.*;
import com.gibbon.radakan.ai.ArtificialIntelligence;
import com.gibbon.radakan.ai.Behavior;
import com.gibbon.radakan.ai.Nothing;
import com.gibbon.radakan.ai.Player;
import com.gibbon.radakan.ai.Skill;
import com.gibbon.radakan.ai.StrategyStateMachine;
import com.gibbon.radakan.entity.Entity.Type;

///	All characters observe each other.
public final class Character  {
	
	public Character (Entity newEntity, Type type, String newSpecie) {
		entity = newEntity;

		Behavior behavior = null;
		if(type == Type.PLAIN) {
			behavior = new Player(entity);
		}
		else if(type == Type.PLAIN) {
			behavior = new ArtificialIntelligence(entity);
		}
		stateMachine = new StrategyStateMachine <Nothing, Behavior> (behavior);
		
		if (type == Type.NPC || type == Type.PLAYER_CHARACTER) {
			skills = new HashMap<String, Skill>();
			equipment = new Equipment(entity.name);
		} else {
			skills = null;
			equipment = null;
		}
		
		specie = newSpecie;
	}

	static String Arachnyd;
	static String Grogg;
	static String Harpy;
	static String Inferno;
	static String Xemna;

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
};
