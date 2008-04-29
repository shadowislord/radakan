package com.gibbon.radakan.ai;

/**
 *	EventListener is an abstract base class for the observer pattern.
 */
public interface EventListener <MessageType> {
	public void call(MessageType message);
}
