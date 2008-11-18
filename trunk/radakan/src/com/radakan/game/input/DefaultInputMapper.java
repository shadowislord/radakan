package com.radakan.game.input;

import java.util.HashMap;

import com.jme.input.KeyInput;

public class DefaultInputMapper implements IInputMapper {
	
	private HashMap<Integer, String> keyMappings = new HashMap<Integer, String>();
	
	public DefaultInputMapper() {
		
	}

	@Override
	public InputActionEvent mapKeyInput(char character, int keyCode, boolean pressed) {
		switch(keyCode) {
			case KeyInput.KEY_D:
			case KeyInput.KEY_RIGHT:
				if(pressed)
					return new InputActionEvent(InputActions.CAMERA_ROTATE_LEFT, -1, -1, 1);
			case KeyInput.KEY_A:
			case KeyInput.KEY_LEFT:
				if(pressed)
					return new InputActionEvent(InputActions.CAMERA_ROTATE_RIGHT, -1, -1, 1);
			case KeyInput.KEY_W:
			case KeyInput.KEY_UP:
				if(pressed)
					return new InputActionEvent(InputActions.PLAYER_MOVE_FWD, -1, -1, 1);
			case KeyInput.KEY_S:
			case KeyInput.KEY_DOWN:
				if(pressed)
					return new InputActionEvent(InputActions.PLAYER_MOVE_BACK, -1, -1, 1);
			case KeyInput.KEY_E:
				if(pressed)
					return new InputActionEvent(InputActions.CAMERA_ROTATE_UP, -1, -1, 1);
			case KeyInput.KEY_Q:
				if(pressed)
					return new InputActionEvent(InputActions.CAMERA_ROTATE_DOWN, -1, -1, 1);
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
