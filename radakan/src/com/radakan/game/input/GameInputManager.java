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

	public boolean showConfigDialog() {
		GameSettings settings = Game.getConfigManager().getSettings();

		ControlSettingsDialog dialog = new ControlSettingsDialog(inputManager,
				settings);
		dialog.open();
		try {
			dialog.waitFor();
		} catch (InterruptedException ex) {
			Game.getDebugManager().reportError(
					"Interrupted while waiting for dialog to close", ex);
		}
		return dialog.isAccepted();
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
			boolean handled = false;

			IGameInputListener listener = listeners.get(InputType.GUI);
			if (listener != null) {
				if (listener instanceof IGameRawInputListener) {
					handled = ((IGameRawInputListener) listener).onButtonPress(
							button, pressed, x, y);
				} else if (listener instanceof IGameActionListener) {
					// TODO Fill in
				}
			}

			if (!handled) {
				listener = listeners.get(InputType.PLAYER);
				if (listener != null) {
					if (listener instanceof IGameRawInputListener) {
						handled = ((IGameRawInputListener) listener)
								.onButtonPress(button, pressed, x, y);
					} else if (listener instanceof IGameActionListener) {
						// TODO Fill in
					}
				}
			}
			
			if (!handled) {
				listener = listeners.get(InputType.CAMERA);
				if (listener != null) {
					if (listener instanceof IGameRawInputListener) {
						handled = ((IGameRawInputListener) listener)
								.onButtonPress(button, pressed, x, y);
					} else if (listener instanceof IGameActionListener) {
						// TODO Fill in
					}
				}
			}

			if (!handled) {
				listener = listeners.get(InputType.OTHER);
				if (listener != null) {
					if (listener instanceof IGameRawInputListener) {
						handled = ((IGameRawInputListener) listener)
								.onButtonPress(button, pressed, x, y);
					} else if (listener instanceof IGameActionListener) {
						// TODO Fill in
					}
				}
			}
		}

		@Override
		public void onMove(int xDelta, int yDelta, int newX, int newY) {
			boolean handled = false;

			IGameInputListener listener = listeners.get(InputType.GUI);
			if (listener != null) {
				if (listener instanceof IGameRawInputListener) {
					handled = ((IGameRawInputListener) listener).onMove(xDelta, yDelta, newX, newY);
				} else if (listener instanceof IGameActionListener) {
					// TODO Fill in
				}
			}

			if (!handled) {
				listener = listeners.get(InputType.PLAYER);
				if (listener != null) {
					if (listener instanceof IGameRawInputListener) {
						handled = ((IGameRawInputListener) listener).onMove(xDelta, yDelta, newX, newY);
					} else if (listener instanceof IGameActionListener) {
						// TODO Fill in
					}
				}
			}
			
			if (!handled) {
				listener = listeners.get(InputType.CAMERA);
				if (listener != null) {
					if (listener instanceof IGameRawInputListener) {
						handled = ((IGameRawInputListener) listener).onMove(xDelta, yDelta, newX, newY);
					} else if (listener instanceof IGameActionListener) {
						// TODO Fill in
					}
				}
			}

			if (!handled) {
				listener = listeners.get(InputType.OTHER);
				if (listener != null) {
					if (listener instanceof IGameRawInputListener) {
						handled = ((IGameRawInputListener) listener).onMove(xDelta, yDelta, newX, newY);
					} else if (listener instanceof IGameActionListener) {
						// TODO Fill in
					}
				}
			}
		}

		@Override
		public void onWheel(int wheelDelta, int x, int y) {
			boolean handled = false;

			IGameInputListener listener = listeners.get(InputType.GUI);
			if (listener != null) {
				if (listener instanceof IGameRawInputListener) {
					handled = ((IGameRawInputListener) listener).onWheel(wheelDelta, x, y);
				} else if (listener instanceof IGameActionListener) {
					// TODO Fill in
				}
			}

			if (!handled) {
				listener = listeners.get(InputType.PLAYER);
				if (listener != null) {
					if (listener instanceof IGameRawInputListener) {
						handled = ((IGameRawInputListener) listener).onWheel(wheelDelta, x, y);
					} else if (listener instanceof IGameActionListener) {
						// TODO Fill in
					}
				}
			}
			
			if (!handled) {
				listener = listeners.get(InputType.CAMERA);
				if (listener != null) {
					if (listener instanceof IGameRawInputListener) {
						handled = ((IGameRawInputListener) listener).onWheel(wheelDelta, x, y);
					} else if (listener instanceof IGameActionListener) {
						// TODO Fill in
					}
				}
			}

			if (!handled) {
				listener = listeners.get(InputType.OTHER);
				if (listener != null) {
					if (listener instanceof IGameRawInputListener) {
						handled = ((IGameRawInputListener) listener).onWheel(wheelDelta, x, y);
					} else if (listener instanceof IGameActionListener) {
						// TODO Fill in
					}
				}
			}
		}

		@Override
		public void onKey(char character, int keyCode, boolean pressed) {			
			boolean handled = false;

			IGameInputListener listener = listeners.get(InputType.GUI);
			if (listener != null) {
				if (listener instanceof IGameRawInputListener) {
					handled = ((IGameRawInputListener) listener).onKey(character, keyCode, pressed);
				} else if (listener instanceof IGameActionListener) {
					IGameActionListener list = (IGameActionListener)listener;
					InputActionEvent evt = list.getMapper().mapKeyInput(character, keyCode, pressed);
					if(evt != null) {
						handled = list.onActionTriggered(evt);
					}
				}
			}

			if (!handled) {
				listener = listeners.get(InputType.PLAYER);
				if (listener != null) {
					if (listener instanceof IGameRawInputListener) {
						handled = ((IGameRawInputListener) listener).onKey(character, keyCode, pressed);
					} else if (listener instanceof IGameActionListener) {
						IGameActionListener list = (IGameActionListener)listener;
						InputActionEvent evt = list.getMapper().mapKeyInput(character, keyCode, pressed);
						if(evt != null) {
							handled = list.onActionTriggered(evt);
						}
					}
				}
			}
			
			if (!handled) {
				listener = listeners.get(InputType.CAMERA);
				if (listener != null) {
					if (listener instanceof IGameRawInputListener) {
						handled = ((IGameRawInputListener) listener).onKey(character, keyCode, pressed);
					} else if (listener instanceof IGameActionListener) {
						IGameActionListener list = (IGameActionListener)listener;
						InputActionEvent evt = list.getMapper().mapKeyInput(character, keyCode, pressed);
						if(evt != null) {
							handled = list.onActionTriggered(evt);
						}
					}
				}
			}

			if (!handled) {
				listener = listeners.get(InputType.OTHER);
				if (listener != null) {
					if (listener instanceof IGameRawInputListener) {
						handled = ((IGameRawInputListener) listener).onKey(character, keyCode, pressed);
					} else if (listener instanceof IGameActionListener) {
						IGameActionListener list = (IGameActionListener)listener;
						InputActionEvent evt = list.getMapper().mapKeyInput(character, keyCode, pressed);
						if(evt != null) {
							handled = list.onActionTriggered(evt);
						}
					}
				}
			}
		}

	}
}
