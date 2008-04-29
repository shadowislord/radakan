package com.gibbon.radakan.ai;

import java.util.*;

/**
 *
 * @author t
 */

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
