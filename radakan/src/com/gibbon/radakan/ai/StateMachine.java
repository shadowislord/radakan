/*
 * Radakan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.gibbon.radakan.ai;

import java.util.Vector;

///	StateMachine is the abstract base class for all state machines.
class StateMachine <StateType> {
	public StateMachine (StateType newState) {
		setState(newState);
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
