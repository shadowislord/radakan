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

import java.io.IOException;
import java.net.URISyntaxException;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.prefs.Preferences;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.gibbon.jme.pass.ExitListenerPass;
import com.gibbon.jme.pass.InputPass;

import com.jme.system.GameSettings;
import com.jme.system.PreferencesGameSettings;
import com.jme.util.resource.ResourceLocator;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;

import com.radakan.gui.console.JmeConsole;
import com.radakan.gui.console.ScriptSystem;
import com.radakan.gui.dialogs.GameSettingsDialog;
import com.radakan.util.ErrorHandler;

/**
 * @author Joshua Montgomery
 * @version 1.0.0
 * @created Aug 18, 2008
 */
public class RadakanGame
{
	private static Logger logger = Logger.getLogger(Main.class.getName());
	
	/**The GameSettings*/
	private static GameSettings settings;
	  
	/**Determines if the game is in debug mode.*/
    private static boolean debugMode;
	    
    /**Constructor - Constructs the game.*/
    public RadakanGame()
    {    
    	if(settings == null)
    		 settings = new PreferencesGameSettings(Preferences.userRoot().node("Radakan"));
    }
	
    /**Initializes the game.*/
	protected void initGame()
	{
		logger.fine("Initializing the game");
		
		try
		{
            JmeContext context = JmeContext.create(LWJGLContext.class, JmeContext.CONTEXT_WINDOW);
            context.setSettings(Game.getSettings());
            context.start();
            logger.fine("Display started");
            context.waitFor();
            logger.info("Display created successfuly");
            
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
		
		logger.fine("Game initialization finished");		
	}

	/**Initializes the system values needed by the game before it can start.*/
	protected void initSystem()
	{
		logger.fine("Initializing the system...");
		
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
            logger.log(Level.SEVERE, "Interrupt ");
        } catch (IOException ex){
            logger.log(Level.WARNING, "Failed to save settings to system", ex);
        }
        
        logger.fine("Setting up locators");
        try {
            ResourceLocator textureLocator = new SimpleResourceLocator(Main.class.getResource("/com/radakan/data/textures/"));
            ResourceLocator imageLocator = new SimpleResourceLocator(Main.class.getResource("/com/radakan/data/images/"));
            ResourceLocator scriptLocator = new SimpleResourceLocator(Main.class.getResource("/com/radakan/data/scripts/"));
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, textureLocator);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, imageLocator);
            ResourceLocatorTool.addResourceLocator("Script", scriptLocator);
        } catch (URISyntaxException ex){
            ErrorHandler.reportError("Texture directory missing", ex);
        }
		
		logger.fine("System initialization finished!");
	}
	
	/**Performs any final cleanup needed after the game is finished.*/
	public void cleanUp()
	{
		//TODO: Any final cleanup for the game
	}
		
    /**
     * Enable or disable debug mode.
     * 
     * During debug mode, more logging messages are displayed
     * and an in-game console is available.
     */
    public static void setDebug(boolean debug){
        debugMode = debug;
        
        if (debug){
            Logger.getLogger("").setLevel(Level.FINEST);
        }else{
            Logger.getLogger("").setLevel(Level.WARNING);
        }
    }
    
    /**
     * Returns true if debug mode is set.
     */
    public static boolean isDebug(){
        return debugMode;
    }
}
