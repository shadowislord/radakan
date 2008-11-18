/*
 * Radakan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.gibbon.jme.pass;

import com.gibbon.jme.context.JmeContext;
import com.jme.input.InputHandler;
import com.jme.input.KeyInput;
import com.jme.input.MouseInput;
import com.jme.input.joystick.JoystickInput;

/**
 * Updates input and InputHandlers
 * 
 * @author Momoko_Fan
 */
public class InputPass extends Pass {
    
    protected boolean cursor;
    
    public InputPass(boolean cursorVisible){
        super(PassType.INPUT,"Input");
        cursor = cursorVisible;
    }
    public void runPass(JmeContext context) {
        KeyInput.get().update();
        MouseInput.get().update();
        JoystickInput.get().update();
    }
    
    public void initPass(JmeContext context) {
  
        if (cursor)
            MouseInput.get().setCursorVisible(true);
    }

    @Override
    public void cleanPass(JmeContext context) {
        KeyInput.destroyIfInitalized();
        MouseInput.destroyIfInitalized();
        JoystickInput.destroyIfInitalized();
    }
}
