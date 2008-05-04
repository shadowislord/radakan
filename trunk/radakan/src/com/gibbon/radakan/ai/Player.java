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

import com.gibbon.radakan.entity.Entity;

public final class Player extends Behavior {
	///	in radians
	private static float minVerticalCameraAngle = - (float)Math.PI/2;
	
	///	in radians
	private static float maxVerticalCameraAngle = (float)Math.PI/2;

	public Player(Entity newEntity) {
		super(newEntity);
	}

	///	The message is showed.
	public void call (Communication message) {
		//	The NPCs message are showed in the log, to let the player know.
		System.out.println(message.from.name + ": " + message.name);

		if(message.name == "Bye.") {
			//	Deselect the NPC that was conversating with you.
			target = null;
		}
	}

	public Behavior transit (Nothing message) {

		float relativeDestinationMovementSpeed = 0;
/*		if(CommandReader.get()) {
			relativeDestinationMovementSpeed = 1;
			System.out.println("Going forward");
		}
		else if(CommandReader.get()) {
			relativeDestinationMovementSpeed = - 0.7;
			System.out.println("Going backwards: - 0.7");
		}
		item.getBody().walk(relativeDestinationMovementSpeed);

		float relativeDestinationTurnSpeed = 0;
		if(Input.CommandReader.get()) {
			relativeDestinationTurnSpeed = 1;
			System.out.println("Turn left");
		}
		else if(Engines.Input.CommandReader.get()) {
			relativeDestinationTurnSpeed = - 1;
			System.out.println("Turn right");
		}
		item.getBody().turn(relativeDestinationTurnSpeed);

		item.getBody().standUp();

		//	jump
		if(CommandReader.get().hasCommand()) {
			item.getBody().jump();
		}

		//	reset your orientation
		if(CommandReader.get().hasCommand()) {
			item.getBody().reset();
			Engines.Log.show("Your orientation is reset.");
		}

		//	Select a target.
		if(CommandReader.get()) {
			if(target.lock() || target.lock()) {
				target.reset();
				target.reset();
				Log.show("Target deselected.");
			}
			else {
				//	TODO Fix this.
				//target
				//	= World.get().getCloseModels().begin();

				Log.show("Target selected: " + target.name);
			}
		}

		//	Give the weapon to your target or take it back.
		if(CommandReader.get().hasCommand()) {
			if(target != null) {
				ContainerItem <Item> playerHand = item.rightHand;
				ContainerItem <Item> npcHand = target.rightHand;

				if(! playerHand.isEmpty()) {
					playerHand.move(playerHand.getChild(), npcHand);
					Engines.Log.show("You gave your weapon to the ninja.");
				}
				else if(! npcHand.isEmpty()) {
					npcHand.move(npcHand.getChild(), playerHand);
					Log.show("You took your weapon from the ninja.");
				}
				else {
					Log.show("Both you and the ninja don't have a weapon.");
				}
			}
			else {
				Log.show("No target selected.");
			}
		}

		Mathematics.Vector3D mousePosition = MouseReader.get().getRelativeMousePosition();

		if
		(
			Input.MouseReader.get().isMouseButtonPressed
			(
				Input.MouseReader.getMiddleMouseButton(),
				false
			)
		) {
			if(mousePosition.x != 0) {
				item.getBody().turn(- Ogre.Math.Sign(mousePosition.x));
			}

			if(mousePosition.y != 0) {
				verticalCameraAngle -= 0.01 * mousePosition.y;
			}
		}

		if(mousePosition.z != 0) {
			cameraDistance += - 0.0005 * mousePosition.z;
		}*/
		
		return (Behavior)this;
	}

/*	public Boolean evaluateCondition(TiXmlElement element) {
		
		Boolean result = true;

		for(TiXmlAttribute attribute = element.FirstAttribute(); attribute != null;
			attribute = attribute.Next()) {
			result = result && evaluateExpression(attribute);
		}

		return result;
	}
	public Boolean evaluateExpression(TiXmlAttribute attribute) {

		String name = attribute.NameTStr();
		String value = attribute.ValueStr();
		System.out.println("evaluate: " << name  << " ? " << value);
		
		if((name == "knows") ||(name == "has")) {
			Engines.Log.log(this)
				<< "result: false(I don't know and I don't have anything.)");
			
			return false;
		}

		
		Boolean larger =(value.at(0) == '>');
		String temp = value;
		temp.erase(temp.begin());
		float numericValue = toFloat(temp);

		Boolean isSmaller =(numericValue < item.getSkill(name));

		System.out.println("result: " << toString(isSmaller != larger));
		return(isSmaller != larger);
	}
	
	public Set <TiXmlDocument> getBehaviorFiles() {
		//	Return an empty set.
		return <HashSet <TiXmlDocument> >(new HashSet <TiXmlDocument>);
	}
	
	public Mathematics.Vector3D getCameraPosition() {

		return cameraDistance * item.getBody().getTopDirection()
			+ item.getBody().getPosition();
	}
	public Mathematics.Quaternion getCameraOrientation() {

		return Mathematics.Quaternion
			(verticalCameraAngle.getValue(),
				item.getBody().getSideDirection())
			* item.getBody().getOrientation();
	}
	
	public void listCommunicationOptions() {
		//	Note: no target gives no options.
		if(target != null && target.isAlive()) {
			if(target != item) {
				Set <TiXmlDocument> behaviorFiles
					= target.getState().getBehaviorFiles();

				for(Set <TiXmlDocument>.iterator file = behaviorFiles.begin();
					file != behaviorFiles.end(); file ++) {
					loadCommunicationOptions(file.RootElement());
				}
			}
		}
	}
	
	private void loadCommunicationOptions(TiXmlElement element) {
		if(element.ValueStr() == "option") {
			Mediator.communicationMediator.callListeners
					(Communication
						(new Converse (item, target, element)));
		}
		
		if((element.ValueStr() == "reactions")) {
			return;
		}
		
		if((element.ValueStr() == "ifNpc")
			&& ! target.getState().evaluateCondition(element)) {
			return;
		}
		
		if((element.ValueStr() == "ifPlayer") && ! evaluateCondition(element)) {
			return;
		}
		
		for(TiXmlElement subElement = element.FirstChildElement();
			subElement != null; subElement = subElement.NextSiblingElement()) {
			loadCommunicationOptions(subElement);
		}
	}*/
		
	private float cameraDistance = 0.72f;
	

	private float verticalCameraAngle = 0;
};

