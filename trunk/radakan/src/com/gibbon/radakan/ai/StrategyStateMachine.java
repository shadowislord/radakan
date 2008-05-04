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

///	I'm the abstract base class for all strategy based state machines.
///	Derive me for each class that uses strategies.
public class StrategyStateMachine<MessageType, StrategyType extends Strategy <MessageType, StrategyType> > 
		extends StateMachine<StrategyType> {
	
	public StrategyStateMachine (StrategyType newState){
		super(newState);
	}
	
	public final void run(MessageType message) {
		if(hasState()) {
			setState(getState().transit(message));
		}
	};
}
