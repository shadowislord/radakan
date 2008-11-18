package com.radakan.game.input;

public interface IGameActionListener extends IGameInputListener {
	public boolean onActionTriggered(InputActionEvent action);
	
	public IInputMapper getMapper();
}
