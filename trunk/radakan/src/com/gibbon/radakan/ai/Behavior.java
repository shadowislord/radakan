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