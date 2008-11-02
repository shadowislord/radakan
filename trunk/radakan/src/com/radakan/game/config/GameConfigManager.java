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

public class GameConfigManager {

    private static Logger logger = Logger.getLogger(GameConfigManager.class.getName());
    
    private GameSettings settings;
    
    /**
     * Initializes configuration.
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
            settings.setSamples(0);
            settings.setDepthBits(8);
            settings.setAlphaBits(0);
            settings.setStencilBits(0);
            settings.setFramerate(-1);
            
            settings.set("title", GameSysInfoManager.GAME_NAME + " " + 
                                  GameSysInfoManager.GAME_SUFFIX + " " +
                                  GameSysInfoManager.GAME_VERSION);
        }
    }
    
    /**
     * Settings for the game. Cannot be null.
     */
    public GameSettings getSettings(){
        return settings;
    }
    
}
