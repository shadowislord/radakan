package com.radakan.game.input;

import com.gibbon.jme.pass.ExitListenerPass;
import com.gibbon.jme.pass.InputPass;
import com.jme.input.KeyInput;
import com.jme.input.KeyInputListener;
import com.jme.input.MouseInput;
import com.jme.input.MouseInputListener;
import com.jme.input.controls.GameControlManager;
import com.jme.system.GameSettings;
import com.radakan.game.Game;
import com.radakan.gui.dialogs.ControlSettingsDialog;

import com.gibbon.jme.context.*;

import java.util.HashMap;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.lwjgl.input.Keyboard;

public class GameInputManager {

	Logger logger = Logger.getLogger(GameInputManager.class.getName());
	
	private GameControlManager inputManager = new GameControlManager();
	private HashMap<InputType, IGameInputListener> listeners = new HashMap<InputType, IGameInputListener>();

	private InputPass inputPass;
	private InputListener listener;

	/**
	 * Depends on: - GameGUIManager
	 */
	public GameInputManager() {
		listener = new InputListener();
		
		inputPass = new InputPass(true);
		JmeContext.get().getPassManager().add(inputPass);
		
		KeyInput.setProvider(LWJGLKeyInput2.class);
		KeyInput.get().addListener(listener);
		MouseInput.get().addListener(listener);

		// TODO Remove ExitListener since it doesn't play nicely with our input handling
		ExitListenerPass exitListener = new ExitListenerPass();
		JmeContext.get().getPassManager().add(exitListener);
	}

	public void setEnabled(boolean enable) {
		inputPass.setEnabled(enable);
	}

	public void setInputListener(IGameInputListener listener, InputType type) {
		this.listeners.put(type, listener);
	}
	
	public void removeInputListener(InputType type) {
		this.listeners.remove(type);
	}

	class InputListener implements MouseInputListener, KeyInputListener {

		@Override
		public void onButton(int button, boolean pressed, int x, int y) {
			boolean handled = handleOnButton(button, pressed, x, y, listeners.get(InputType.GUI));
			
			if(!handled)
				handled = handleOnButton(button, pressed, x, y, listeners.get(InputType.PLAYER));
			
			if(!handled)
				handled = handleOnButton(button, pressed, x, y, listeners.get(InputType.CAMERA));
			
			if(!handled)
				handled = handleOnButton(button, pressed, x, y, listeners.get(InputType.OTHER));
		}

		@Override
		public void onMove(int xDelta, int yDelta, int newX, int newY) {
			boolean handled = handleOnMove(xDelta, yDelta, newX, newY, listeners.get(InputType.GUI));
			
			if(!handled)
				handled = handleOnMove(xDelta, yDelta, newX, newY, listeners.get(InputType.PLAYER));
			
			if(!handled)
				handled = handleOnMove(xDelta, yDelta, newX, newY, listeners.get(InputType.CAMERA));
			
			if(!handled)
				handled = handleOnMove(xDelta, yDelta, newX, newY, listeners.get(InputType.OTHER));
		}

		@Override
		public void onWheel(int wheelDelta, int x, int y) {
			boolean handled = handleOnWheel(wheelDelta, x, y, listeners.get(InputType.GUI));
			
			if(!handled)
				handled = handleOnWheel(wheelDelta, x, y, listeners.get(InputType.PLAYER));
			
			if(!handled)
				handled = handleOnWheel(wheelDelta, x, y, listeners.get(InputType.CAMERA));
				
			if(!handled)
				handled = handleOnWheel(wheelDelta, x, y, listeners.get(InputType.OTHER));
		}

		@Override
		public void onKey(char character, int keyCode, boolean pressed) {			
			boolean handled = handleOnKey(character, keyCode, pressed, listeners.get(InputType.GUI));

			if (!handled) 
				handled = handleOnKey(character, keyCode, pressed, listeners.get(InputType.PLAYER));
			
			if (!handled)
				handled = handleOnKey(character, keyCode, pressed, listeners.get(InputType.CAMERA));

			if (!handled)
				handled = handleOnKey(character, keyCode, pressed, listeners.get(InputType.OTHER));
		}
		
		private boolean handleOnKey(char character, int keyCode, boolean pressed, IGameInputListener listener) {			
			if (listener != null) {
				if (listener instanceof IGameRawInputListener) {
					return ((IGameRawInputListener) listener).onKey(character, keyCode, pressed);
				} else if (listener instanceof IGameActionListener) {
					IGameActionListener list = (IGameActionListener)listener;
					InputActionEvent evt = list.getMapper().mapKeyInput(character, keyCode, pressed);
					if(evt != null) {
						return list.onActionTriggered(evt);
					}
				}
			}
			
			return false;
		}
		
		private boolean handleOnButton(int button, boolean pressed, int x, int y, IGameInputListener listener) {			
			if (listener != null) {
				if (listener instanceof IGameRawInputListener) {
					return ((IGameRawInputListener) listener).onButtonPress(button, pressed, x, y);
				} else if (listener instanceof IGameActionListener) {
					IGameActionListener list = (IGameActionListener)listener;
					InputActionEvent evt = list.getMapper().mapMouseButtonInput(button, pressed, x, y);
					if(evt != null) {
						return list.onActionTriggered(evt);
					}
				}
			}
			
			return false;
		}
		
		private boolean handleOnMove(int xDelta, int yDelta, int newX, int newY, IGameInputListener listener) {			
			if (listener != null) {
				if (listener instanceof IGameRawInputListener) {
					return ((IGameRawInputListener) listener).onMove(xDelta, yDelta, newX, newY);
				} else if (listener instanceof IGameActionListener) {
					IGameActionListener list = (IGameActionListener)listener;
					InputActionEvent evt = list.getMapper().mapMouseMove(xDelta, yDelta, newX, newY);
					if(evt != null) {
						return list.onActionTriggered(evt);
					}
				}
			}
			
			return false;
		}
		
		private boolean handleOnWheel(int wheelDelta, int x, int y, IGameInputListener listener) {			
			if (listener != null) {
				if (listener instanceof IGameRawInputListener) {
					return ((IGameRawInputListener) listener).onWheel(wheelDelta, x, y);
				} else if (listener instanceof IGameActionListener) {
					IGameActionListener list = (IGameActionListener)listener;
					InputActionEvent evt = list.getMapper().mapMouseWheelInput(wheelDelta, x, y);
					if(evt != null) {
						return list.onActionTriggered(evt);
					}
				}
			}
			
			return false;
		}
	}
}
