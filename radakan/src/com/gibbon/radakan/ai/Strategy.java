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
