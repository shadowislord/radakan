package com.gibbon.test;

import java.util.concurrent.Callable;
import java.util.prefs.Preferences;

import com.gibbon.jme.context.ExitListenerPass;
import com.gibbon.jme.context.InputPass;
import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.gibbon.radakan.Setup;
import com.gibbon.radakan.config.ConfigFrame;
import com.gibbon.radakan.error.ErrorReporter;
import com.gibbon.radakan.ui.MainMenu2;
import com.gibbon.radakan.ui.UIManager;
import com.jme.system.GameSettings;
import com.jme.system.PreferencesGameSettings;


public class TestUI2 {

    public static void initGame(JmeContext cx){
        // create gui
        Setup.loadGuiPass(cx);
        UIManager.setXMLTheme("data/themes/QtCurve.xml");
        
        MainMenu2 menu = new MainMenu2();
        UIManager.setContext(menu, false);
    }
    
    public static void main(String[] args){
        //Logger.getLogger("").setLevel(Level.WARNING);
        
        GameSettings settings
                = new PreferencesGameSettings(Preferences.userRoot().node("test-jmecontext"));
        
        settings.set("title", "Test Main Menu");
        
        JmeContext context = null;
        try{
            ConfigFrame g = new ConfigFrame(settings);
            g.setVisible(true);
            g.waitFor();

//            settings.setWidth(720);
//            settings.setHeight(480);
            
            context = JmeContext.create(LWJGLContext.class, JmeContext.CONTEXT_WINDOW);
            context.setSettings(settings);
            context.start();
            context.waitFor();
        } catch (Exception ex){
            ErrorReporter.reportError("Error while initializing display", ex);
        }
        
        final JmeContext cx = context;
        
        // add input pass
        InputPass input = new InputPass(null, true);
        cx.getPassManager().add(input);
        
        // add exit listener
        ExitListenerPass elp = new ExitListenerPass(new Callable<Object>(){
            public Object call() throws Exception {
                cx.dispose();
                System.exit(0);
                return null;
            }
        });
        cx.getPassManager().add(elp);


        initGame(cx);
    }
    
}
