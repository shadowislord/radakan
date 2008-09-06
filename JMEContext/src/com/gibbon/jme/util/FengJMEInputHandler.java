package com.gibbon.jme.util;

import org.fenggui.Display;

import com.jme.input.InputHandler;
import com.jme.input.MouseInput;
import com.jme.input.action.InputActionEvent;
import com.jme.input.action.KeyInputAction;
import org.fenggui.event.key.Key;
import org.lwjgl.input.Keyboard;

/**
 * FengJMEInputHandler
 *
 * @author Joshua Keplinger
 *
 */
public class FengJMEInputHandler extends InputHandler {

    private Display disp;
    private KeyInputAction keyAction;
    private boolean keyHandled;
    private boolean mouseHandled;

    public FengJMEInputHandler(Display disp) {
        this.disp = disp;

        //keyAction = new KeyAction();
        //addAction(keyAction, DEVICE_KEYBOARD, BUTTON_ALL, AXIS_NONE, false);

        MouseInput.get().addListener(new MouseListener(disp));
    }

    @Override
    public void update(float time) {
        keyHandled = false;
        mouseHandled = false;
        super.update(time);
    }

    public boolean wasKeyHandled() {
        return keyHandled;
    }

    public boolean wasMouseHandled() {
        return mouseHandled;
    }

	private class KeyAction extends KeyInputAction
	{
 
		public void performAction(InputActionEvent evt)
		{
			char character = evt.getTriggerCharacter();
			Key key = mapKeyEvent();
			if(evt.getTriggerPressed()) {
				keyHandled = disp.fireKeyPressedEvent(character, key);
				// Bug workaround see note after code
				if (key == Key.LETTER || key == Key.DIGIT)
					keyHandled = disp.fireKeyTypedEvent(character);
			} else
				keyHandled = disp.fireKeyReleasedEvent(character, key);
		}
 
		/**
		 * Helper method that maps LWJGL key events to FengGUI.
		 * @return The Key enumeration of the last key pressed.
		 */
		private Key mapKeyEvent()
		{
			Key keyClass;
 
	        switch(Keyboard.getEventKey())
	        {
		        case Keyboard.KEY_BACK:
		        	keyClass = Key.BACKSPACE;
		            break;
		        case Keyboard.KEY_RETURN:
		        	keyClass = Key.ENTER;
		            break;
		        case Keyboard.KEY_DELETE:
		        	keyClass = Key.DELETE;
		            break;
		        case Keyboard.KEY_UP:
		        	keyClass = Key.UP;
		        	break;
		        case Keyboard.KEY_RIGHT:
		        	keyClass = Key.RIGHT;
		            break;
		        case Keyboard.KEY_LEFT:
		        	keyClass = Key.LEFT;
		            break;
		        case Keyboard.KEY_DOWN:
		        	keyClass = Key.DOWN;
		            break;
		        case Keyboard.KEY_SCROLL:
		        	keyClass = Key.SHIFT;
		            break;
		        case Keyboard.KEY_LMENU:
		        	keyClass = Key.ALT;
		            break;
		        case Keyboard.KEY_RMENU:
		        	keyClass = Key.ALT;
		            break;
		        case Keyboard.KEY_LCONTROL:
		        	keyClass = Key.CTRL;
		            break;
		        case Keyboard.KEY_RSHIFT:
		        	keyClass = Key.SHIFT;
		            break;
		        case Keyboard.KEY_LSHIFT:
		        	keyClass = Key.SHIFT;
		            break;
		        case Keyboard.KEY_RCONTROL:
		        	keyClass = Key.CTRL;
		            break;
		        case Keyboard.KEY_INSERT:
		        	keyClass = Key.INSERT;
		            break;
		        case Keyboard.KEY_F12:
		        	keyClass = Key.F12;
		            break;
		        case Keyboard.KEY_F11:
		        	keyClass = Key.F11;
		            break;
		        case Keyboard.KEY_F10:
		        	keyClass = Key.F10;
		            break;
		        case Keyboard.KEY_F9:
		        	keyClass = Key.F9;
		            break;
		        case Keyboard.KEY_F8:
		        	keyClass = Key.F8;
		            break;
		        case Keyboard.KEY_F7:
		        	keyClass = Key.F7;
		            break;
		        case Keyboard.KEY_F6:
		        	keyClass = Key.F6;
		            break;
		        case Keyboard.KEY_F5:
		        	keyClass = Key.F5;
		            break;
		        case Keyboard.KEY_F4:
		        	keyClass = Key.F4;
		            break;
		        case Keyboard.KEY_F3:
		        	keyClass = Key.F3;
		            break;
		        case Keyboard.KEY_F2:
		        	keyClass = Key.F2;
		            break;
		        case Keyboard.KEY_F1:
		        	keyClass = Key.F1;
		            break;
		        default:
		        	if("1234567890".indexOf(Keyboard.getEventCharacter()) != -1) {
		        		keyClass = Key.DIGIT;
		        	} else {
		        		// @todo must not necessarily be a letter!! #
		        		keyClass = Key.LETTER;
		        	}
		        	break;
	    	}
 
	        return keyClass;
		}
 
	}
    
    
}
