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

public final class Mediator <A> {
	/*	Mediator(){
		mediators.add(Action, new Mediator<Action>());
		mediators.add(Communication, new Mediator<Communication>());
	}*/
	
	public /*static*/ void callListeners(A event) {
        for (EventListener<A> listener : /*mediators.get(A).*/eventListeners)
        {
            listener.call(event);
        }
    }
	public /*static*/ void registerListener(EventListener<A> eventListener) {
		/*mediators.get(A).*/eventListeners.add(eventListener);
    }
    
	private /*static*/ Set<EventListener<A>> eventListeners = new HashSet<EventListener<A>>();
	
	/*private static Map<Class<?>, Mediator<?>> mediators;*/
	public static Mediator<Action> actionMediator = new Mediator<Action>();
	public static Mediator<Communication> communicationMediator = new Mediator<Communication>();
	public static Mediator<Nothing> nothingMediator = new Mediator<Nothing>();
}
