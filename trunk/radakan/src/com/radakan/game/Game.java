package com.radakan.game;

import com.jme.system.GameSettings;
import com.jme.system.PreferencesGameSettings;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.prefs.Preferences;

public class Game {

    private static GameSettings settings;
    
    private static boolean debugMode;
    
    /**
     * Initialize settings
     */
    static {
        settings = new PreferencesGameSettings(Preferences.userRoot().node("Radakan"));
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
    
    /**
     * Settings for the game. Cannot be null.
     */
    public static GameSettings getSettings(){
        return settings;
    }
    
}
