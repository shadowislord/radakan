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

package com.radakan.game.config;

import com.jme.system.GameSettings;
import com.jme.system.PreferencesGameSettings;
import com.radakan.game.debug.GameSysInfoManager;
import java.util.logging.Logger;
import java.util.prefs.BackingStoreException;
import java.util.prefs.Preferences;

/**
 * GameConfigManager maintains the game settings database, 
 * which is stored in registry. Game controls, graphics options, audio options, 
 * and game options should be stored in the Config.
 * 
 * @author Kirill
 */
public class GameConfigManager {

    private static Logger logger = Logger.getLogger(GameConfigManager.class.getName());
    
    private GameSettings settings;
    
    /**
     * Initializes the configuration by loading it from registry,
     * initializes default settings for radakan if not available.
     */
    public GameConfigManager() {
        boolean isNew = true;
        
        // Checks to see if there are preferences stored for the game already.
        try {
            isNew = !Preferences.userRoot().nodeExists("Radakan");
        } catch (BackingStoreException e) {
            logger.warning("Error loading game settings from preferences.");
        }
        settings = new PreferencesGameSettings(Preferences.userRoot().node("Radakan"), isNew);
        logger.fine("Settings loaded from registry");
        
        if(isNew) {
            // Display params
            settings.setWidth(640);
            settings.setHeight(480);
            settings.setFullscreen(false);
            settings.setDepth(32);
            settings.setFrequency(60);
            
            // Misc params
            settings.setFramerate(60);
            settings.setMusic(true);
            settings.setSFX(true);
            settings.setRenderer("LWJGL");
            
            // OGL params
            settings.setVerticalSync(false);
            settings.setSamples(0);
            settings.setDepthBits(8);
            settings.setAlphaBits(0);
            settings.setStencilBits(0);
            
            // Radakan params
            settings.set("title", GameSysInfoManager.GAME_NAME + " " + 
                                  GameSysInfoManager.GAME_SUFFIX + " " +
                                  GameSysInfoManager.GAME_VERSION);
            
            logger.fine("Created new settings");
        }
    }
    
    /**
     * Settings for the game. Cannot be null.
     */
    public GameSettings getSettings(){
        return settings;
    }
    
}
