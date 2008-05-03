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

///	I'm the abstract base class for the strategy pattern.
///	For more information about the strategy pattern,
///	see http://en.wikipedia.org/wiki/StrategyPattern.
///	Use me in combination with the StrategyStateMachine class.
public interface Strategy <MessageType, StrategyType> {
	///	Don't call this method directly,
	///	let it be called by StrategyStateMachine.
	///	'transit(...)' returns the next active strategy state.
	public StrategyType transit (MessageType message);
};
