package com.gibbon.tools.world;

import com.radakan.game.Game;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.prefs.BackingStoreException;
import java.util.prefs.Preferences;

public class EditorSettings {

    private static final Preferences pref;
    
    static {
        pref = Preferences.userRoot().node("WorldTool");
    }
    
    public static void save(){
        try {
            pref.flush();
        } catch (BackingStoreException ex) {
            Game.getDebugManager().reportError("Failed to write editor registry settings", ex);
        }
    }
    
    public static Preferences getPrefs(){
        return pref;
    }
    
}
