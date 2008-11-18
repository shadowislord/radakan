package com.radakan.game.input;

import java.util.HashMap;

public class DefaultInputMapper implements IInputMapper {
	
	private HashMap<Integer, String> keyMappings = new HashMap<Integer, String>();
	
	public DefaultInputMapper() {
		
	}

	@Override
	public InputActionEvent mapKeyInput(char character, int keyCode, boolean pressed) {
		switch(character) {
			case 'a':
				return new InputActionEvent(InputActions.CAMERA_ROTATE_LEFT, -1, -1, 1);
			case 'd':
				return new InputActionEvent(InputActions.CAMERA_ROTATE_RIGHT, -1, -1, 1);
		}
		
		return null;
	}

	@Override
	public InputActionEvent mapMouseButtonInput(int button, boolean pressed, int x,
			int y) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public InputActionEvent mapMouseWheelInput(int wheelDelta, int x, int y) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public InputActionEvent mapMouseMove(int xDelta, int yDelta, int newX,
			int newY) {
		// TODO Auto-generated method stub
		return null;
	}

}
