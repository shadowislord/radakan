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

package com.gibbon.jme.context;

import com.jme.input.KeyInput;
import java.util.concurrent.Callable;

/**
 * Calls some code when the user requests to close the program
 * 
 * @author Momoko_Fan
 */
public class ExitListenerPass extends Pass {
    
    protected Callable<Object> exit_code;
    
    /**
     * @param exit_code The code to execute when a termination is requested (Alt-F4, Exit button, Esc)
     */
    public ExitListenerPass(Callable<Object> exit_code) {
        super(PassType.POST_INPUT, "ExitListener");
        if (exit_code == null){
            this.exit_code =
                    new Callable<Object>(){
                        public Object call(){
                            JmeContext.get().dispose();
                            return null;
                        }
                    }; 
        }else{
            this.exit_code = exit_code;
        }
    }
    
    public ExitListenerPass(){
        this(null);
    }

    public void runPass(JmeContext context) {
        if (context.getDisplay().isClosing() || KeyInput.get().isKeyDown(KeyInput.KEY_ESCAPE))
            try {
                exit_code.call();
            } catch (Exception ex) {
                ex.printStackTrace();
            }
    }

    public void initPass(JmeContext context) {
    }

    @Override
    public void cleanPass(JmeContext context) {
    }
    
}
