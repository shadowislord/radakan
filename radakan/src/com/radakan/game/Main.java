/* Radakan is free software: you can redistribute it and/or modify
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

package com.radakan.game;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.gibbon.jme.pass.ExitListenerPass;
import com.gibbon.jme.pass.InputPass;
import com.jme.system.GameSettings;
import com.jme.util.resource.ResourceLocator;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import com.radakan.gui.console.JmeConsole;
import com.radakan.gui.console.ScriptSystem;
import com.radakan.gui.dialogs.GameSettingsDialog;
import com.radakan.util.ErrorHandler;
import com.radakan.util.SysInfo;
import java.io.IOException;
import java.net.URISyntaxException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * @author Kirill
 */
public class Main {

    private static Logger logger = Logger.getLogger(Main.class.getName());
    
    private void init(){
        ErrorHandler.bindUncaughtExceptionHandler();
        Game.setDebug(true);
        
        GameSettings settings = Game.getSettings();
        logger.fine("Settings loaded from registry");
        
        settings.set("title", Game.getGameName() + " "
                            + Game.getVersionPrefix() + " "
                            + Game.getGameVersion());
        
        settings.setSamples(0);
        settings.setDepthBits(8);
        settings.setAlphaBits(0);
        settings.setStencilBits(0);
        settings.setFramerate(-1);
        
        GameSettingsDialog dialog = new GameSettingsDialog(settings);
        dialog.open();
        
        try{
            dialog.waitFor();
            
            if (!dialog.isInitGameAllowed()){
                logger.fine("Game initialization canceled by user");
                System.exit(0);
            }
            
            settings.save();
            logger.fine("Settings saved to registry");
        } catch (InterruptedException ex) {
        	ErrorHandler.reportError("Interrupted", ex);
        } catch (IOException ex){
        	ErrorHandler.reportError("Failed to save settings to system", ex);
        }
        
        logger.fine("Setting up locators");
        try{
            Game.setupDefaultLocators();
        } catch (IOException ex){
            ErrorHandler.reportError("Error while setting locators", ex);
        }
    }
    
    public void start(){
        try{
            JmeContext context = JmeContext.create(LWJGLContext.class, JmeContext.CONTEXT_WINDOW);
            context.setSettings(Game.getSettings());
            context.start();
            logger.fine("Display started");
            context.waitFor();
            logger.info("Display created successfuly");
            
            Game.querySystemInfo();
            
            InputPass input = new InputPass(null, true);
            context.getPassManager().add(input);
            
            ExitListenerPass exitListener = new ExitListenerPass();
            context.getPassManager().add(exitListener);
            
            JmeConsole jmeConsole = new JmeConsole();
            jmeConsole.addConsoleListener(new ScriptSystem(jmeConsole, true));
            context.getPassManager().add(jmeConsole);
        } catch (InterruptedException ex) {
            ErrorHandler.reportError("Interrupt while creating display", ex);
        } catch (InstantiationException ex){
            ErrorHandler.reportError("Display implementation cannot be loaded", ex);
        }
    }
    
    public static void main(String[] args){
        Main main = new Main();
        main.init();
        main.start();
    }
    
}
