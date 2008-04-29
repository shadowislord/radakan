package com.gibbon.radakan.ai;

///	I'm the abstract base class for all strategy based state machines.
///	Derive me for each class that uses strategies.
class StrategyStateMachine<MessageType, StrategyType extends Strategy <MessageType, StrategyType> > extends StateMachine<StrategyType> {
	public StrategyStateMachine (StrategyType newState){
		super(newState);
	}
	
	public final void run(MessageType message) {
		if(hasState()) {
			setState(getState().transit(message));
		}
	};
}
