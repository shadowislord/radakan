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

package com.gibbon.radakan.ai;

import java.util.*;

///	All characters observe each other.
public final class Character extends ContainerItem <Item> implements EventListener <Nothing> {
	Character(String newName,float newMass, Vector3D newSize, MeshData newMeshData, String type) {
		super
		(
			newName,
			newMass,
			newSize,
			newMeshData
		);
		name = newName;

		
		Behavior behavior;
		if(type == "player character") {
			behavior = new Player(this);
		}
		else {
			behavior = new ArtificialIntelligence(this);
		}
		stateMachine = new StrategyStateMachine <Nothing, Behavior> (behavior);
		
		
		//	head(new StaticItem(name + "'s head", "bar.mesh", 1, 1)),
		//	head(new ContainerItem <Hat>()),
		//	body(new ContainerItem <Shirt>()),
		back
		(
			new ContainerItem <ContainerItem <Item> >
			(
				name + "'s back",
				1,
				Vector3D(0.5, 0.5, 0.3),
				MeshData()
			)
		);
		//	arms(new ContainerItem <Bracer>()),
		rightHand
		(
			new ContainerItem <Item>
			(
				name + "'s right hand",
				1,
				Mathematics.Vector3D(1, 0.3, 0.3),
				MeshData()
			)
		);
		//	legs(new ContainerItem <Pants>()),
		//	feet(new ContainerItem <Shoe>()),
		skills(new map <String, <Skill> >) (
			newSize.toString(), newMeshData.name);
		
		Boolean check/* = ContainerItem <Item>.insert(head)*/;
		//	ContainerItem <Item>.insert(body);
		check = ContainerItem <Item>.insert(back);
		//	ContainerItem <Item>.insert(arms);
		check = ContainerItem <Item>.insert(rightHand);
		//	ContainerItem <Item>.insert(legs);
		//	ContainerItem <Item>.insert(feet);
	
		//	Make sure no body parts are added anymore.
		seal();
	
		Mediator.nothingMediator.registerObserver(this);

	}

	public final StrategyStateMachine <Nothing, Behavior> stateMachine;
		
	public static Character getPlayerCharacter() {
		return Player.get().character;
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

	float getSkill(String skillName)	{

		Map <String, Skill>.iterator skill = skills.find(skillName);

		//	when the skill couldn't be found
		if(skill == skills.end()) {
			skill = skills.insert
			(
				pair <String, Skill>
					(skillName, new Skill(skillName))
			).first;
		}

		return skill.second.getValue();
	}
	
	public final String name;

	public String specie;

	//	public Item & head;
	//	public Multislot <Shirt> & body;
	public ContainerItem <ContainerItem <Item> > back;
	//	public Multislot <Bracer> & arms;
	public ContainerItem <Item> leftHand;
	public ContainerItem <Item> rightHand;
	//	public Multislot <Pants> & legs;
	//	public Multislot <Shoe> & feet;

	public Map<String, Skill> skills = new HashMap<String, Skill>();
};
