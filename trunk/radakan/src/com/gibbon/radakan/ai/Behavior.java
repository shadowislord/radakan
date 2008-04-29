package com.gibbon.radakan.ai;

/*import java.util.*;*/

public abstract class Behavior implements EventListener<Communication>, Strategy<Nothing, Behavior> {

	public Behavior(Character newCharacter) {
		character = newCharacter;
        
		Mediator.communicationMediator.registerListener((EventListener<Communication>)this);
	}

	public abstract void call (Communication message);

	public abstract Behavior transit (Nothing message);

	/*public Boolean evaluateCondition(TiXmlElement * element);
	public Boolean evaluateExpression(TiXmlAttribute * attribute);

	public HashSet <TiXmlDocument> getBehaviorFiles();*/

	final public Character character;

	///	At least one of the targets shouldn't target anything.
	public Character characterTarget;
	/*private Item itemTarget;*/
};
