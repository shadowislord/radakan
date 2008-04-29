package com.gibbon.radakan.ai;

//	import java.util.*;

public abstract class Action implements Strategy <Communication, Action> {
	public Action(Character newCharacter, ArtificialIntelligence newAi) {
		character = newCharacter;
		ai = newAi;
	}
	
	public abstract Action transit(Communication message);
	///	'getActionName()' returns the name of a subclass.
	public abstract String getActionName();

	public final Character character;
	public final ArtificialIntelligence ai;
};
