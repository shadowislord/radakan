package com.gibbon.test;

import com.gibbon.jme.context.FengGuiPass;
import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.RenderPass;
import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.gibbon.radakan.config.ConfigFrame;
import com.gibbon.radakan.error.ErrorReporter;
import com.jme.system.GameSettings;
import com.jme.system.PreferencesGameSettings;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.prefs.Preferences;

public class TestUI {

    public static void main(String[] args){
        Logger.getLogger("").setLevel(Level.WARNING);
        
        GameSettings settings
                = new PreferencesGameSettings(Preferences.userRoot().node("test-jmecontext"));
        
        settings.set("title", "Test User Interface");
        
        JmeContext cx = null;
        try{
            ConfigFrame g = new ConfigFrame(settings);
            g.setVisible(true);
            g.waitFor();

            cx = JmeContext.create(LWJGLContext.class, JmeContext.CONTEXT_WINDOW);
            cx.setSettings(settings);
            cx.start();
            cx.waitFor();
        } catch (Exception ex){
            ErrorReporter.reportError("Error while initializing display", ex);
        }
        
        cx.getPassManager().loadDefaultPasses();
        
        FengGuiPass ui = new FengGuiPass();
        cx.getPassManager().add(ui);
    }
    
}
