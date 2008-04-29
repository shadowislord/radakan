package com.gibbon.radakan.ai;

public final class Player extends Behavior {
	///	in radians
	private static float minVerticalCameraAngle;
	
	///	in radians
	private static float maxVerticalCameraAngle;

	public Player(Character newCharacter) {
		super(newCharacter);
		cameraDistance(0.72);
		verticalCameraAngle(0, - Ogre.Math.HALFPI, Ogre.Math.HALFPI);

		Boolean check = character.back.insert(ContainerItem<Item>(new ContainerItem <Item>("Backpack", 3, Mathematics.Vector3D(0.3, 0.5, 0.2), MeshData())));

		check = character.rightHand.insert(Item(new Weapon("Sword", 4, Mathematics.Vector3D(0.1, 0.2, 0.3), MeshData(), 5, 6, 7, 8, 9, 10)));
	}

	///	The message is showed.
	public void call (Communication message) {
		//	The NPCs message are showed in the log, to let the player know.
		Log.show(message.from.name + ": " + message.name);

		if(message.name == "Bye.") {
			//	Deselect the NPC that was conversating with you.
			characterTarget.reset();
		}
	}

	public Behavior transit (Nothing message) {

		float relativeDestinationMovementSpeed = 0;
		if(Engines.Input.CommandReader.get()) {
			relativeDestinationMovementSpeed = 1;
			Engines.Log.log(this) << "Going forward" << endl;
		}
		else if(Engines.Input.CommandReader.get()) {
			relativeDestinationMovementSpeed = - 0.7;
			Engines.Log.log(this) << "Going backwards: - 0.7" << endl;
		}
		character.getBody().walk(relativeDestinationMovementSpeed);

		float relativeDestinationTurnSpeed = 0;
		if(Input.CommandReader.get()) {
			relativeDestinationTurnSpeed = 1;
			Log.log(this) << "Turn left" << endl;
		}
		else if(Engines.Input.CommandReader.get()) {
			relativeDestinationTurnSpeed = - 1;
			Log.log(this) << "Turn right" << endl;
		}
		character.getBody().turn(relativeDestinationTurnSpeed);

		character.getBody().standUp();

		//	jump
		if(CommandReader.get().hasCommand()) {
			character.getBody().jump();
		}

		//	reset your orientation
		if(CommandReader.get().hasCommand()) {
			character.getBody().reset();
			Engines.Log.show("Your orientation is reset.");
		}

		//	Select a target.
		if(CommandReader.get()) {
			if(characterTarget.lock() || itemTarget.lock()) {
				characterTarget.reset();
				itemTarget.reset();
				Log.show("Target deselected.");
			}
			else {
				//	TODO Fix this.
				//characterTarget
				//	= World.get().getCloseModels().begin();

				Log.show("Target selected: " + characterTarget.name);
			}
		}

		//	Give the weapon to your target or take it back.
		if(CommandReader.get().hasCommand()) {
			if(characterTarget != null) {
				ContainerItem <Item> playerHand = character.rightHand;
				ContainerItem <Item> npcHand = characterTarget.rightHand;

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
				character.getBody().turn(- Ogre.Math.Sign(mousePosition.x));
			}

			if(mousePosition.y != 0) {
				verticalCameraAngle -= 0.01 * mousePosition.y;
			}
		}

		if(mousePosition.z != 0) {
			cameraDistance += - 0.0005 * mousePosition.z;
		}
		
		return (Behavior)this;
	}

	public Boolean evaluateCondition(TiXmlElement element) {
		
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
		Log.log(this) << "evaluate: " << name  << " ? " << value << endl;
		
		if((name == "knows") ||(name == "has")) {
			Engines.Log.log(this)
				<< "result: false(I don't know and I don't have anything.)" << endl;
			
			return false;
		}

		
		Boolean larger =(value.at(0) == '>');
		String temp = value;
		temp.erase(temp.begin());
		float numericValue = toFloat(temp);

		Boolean isSmaller =(numericValue < character.getSkill(name));

		Log.log(this) << "result: " << toString(isSmaller != larger) << endl;
		return(isSmaller != larger);
	}
	
	/*public Set <TiXmlDocument> getBehaviorFiles() {
		//	Return an empty set.
		return <HashSet <TiXmlDocument> >(new HashSet <TiXmlDocument>);
	}*/
	
	public Mathematics.Vector3D getCameraPosition() {

		return cameraDistance * character.getBody().getTopDirection()
			+ character.getBody().getPosition();
	}
	public Mathematics.Quaternion getCameraOrientation() {

		return Mathematics.Quaternion
			(verticalCameraAngle.getValue(),
				character.getBody().getSideDirection())
			* character.getBody().getOrientation();
	}
	
	public void listCommunicationOptions() {
		//	Note: no target gives no options.
		if(characterTarget != null && characterTarget.isAlive()) {
			if(characterTarget != character) {
				Set <TiXmlDocument> behaviorFiles
					= characterTarget.getState().getBehaviorFiles();

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
						(new Converse (character, characterTarget, element)));
		}
		
		if((element.ValueStr() == "reactions")) {
			return;
		}
		
		if((element.ValueStr() == "ifNpc")
			&& ! characterTarget.getState().evaluateCondition(element)) {
			return;
		}
		
		if((element.ValueStr() == "ifPlayer") && ! evaluateCondition(element)) {
			return;
		}
		
		for(TiXmlElement subElement = element.FirstChildElement();
			subElement != null; subElement = subElement.NextSiblingElement()) {
			loadCommunicationOptions(subElement);
		}
	}
		
	private float cameraDistance = 0.72;
	
	///	in radians
	private Mathematics.BoundedFloat verticalCameraAngle;
};


