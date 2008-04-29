package com.gibbon.radakan.ai;

import java.util.Vector;

///	StateMachine is the abstract base class for all state machines.
class StateMachine <StateType> {
	public StateMachine (StateType newState) {
		state = newState;
	}
	
	public Boolean hasState(){
		return state != null;
	}

	///	'getState()' requires an active state.
	///	'getState()' will not be null.
	public StateType getState(){
		return state;
	}

	///	'newState' should not be null.
	protected void setState(StateType newState) {
		if(state != newState) {
			if(hasState()) {
				formerStates.add(state.toString());

				System.out.println("'" + state + "' was removed as my active state.");
			}

			state = newState;

			if(hasState()) {
				System.out.println("My active state changed to '" + state + "'.");
			}
		}
	}

	///	'formerStates' stores the name of previous 'state's.
	///	The indices are in chronological order. (0 means first.)
	Vector <String> formerStates = new Vector <String> ();

	private StateType state;
};
