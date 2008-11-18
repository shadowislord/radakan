package com.radakan.game.input;

public interface IInputMapper {
	public InputActionEvent mapKeyInput(char character, int keyCode, boolean pressed);
	
	public InputActionEvent mapMouseButtonInput(int button, boolean pressed, int x, int y);
	
	public InputActionEvent mapMouseWheelInput(int wheelDelta, int x, int y);
	
	public InputActionEvent mapMouseMove(int xDelta, int yDelta, int newX, int newY);
}
