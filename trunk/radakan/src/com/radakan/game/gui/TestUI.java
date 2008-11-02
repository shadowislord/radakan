package com.radakan.game.gui;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.gibbon.jme.pass.ExitListenerPass;
import com.gibbon.jme.pass.FengGuiPass;
import com.gibbon.jme.pass.InputPass;
import com.jme.input.Mouse;
import com.jme.renderer.ColorRGBA;
import com.jme.system.GameSettings;
import com.jme.system.PreferencesGameSettings;
import com.radakan.game.Game;
import com.radakan.gui.dialogs.GameSettingsDialog;
import com.radakan.game.debug.GameDebugManager;
import java.io.IOException;
import java.util.concurrent.Callable;
import java.util.prefs.Preferences;
import org.lwjgl.input.Cursor;

public class TestUI {

    public static void initGame(final JmeContext cx){
        //UIContext ui = new LoadScreen(new GameLoadTask(), true);
        //UIContext ui = new MainMenu();
        UIContext ui = new StartScreen();
        GameGUIManager.setContext(ui, true);
    }
    
    public static void main(String[] args) throws IOException{
        GameSettings settings = Game.getSettings();
        Game.setupDefaultLocators();
        Game.setDebug(true);
        settings.set("title", "Test Main Menu");
        
        JmeContext context = null;
        try{
            GameSettingsDialog dialog = new GameSettingsDialog(settings);
            dialog.open();
            dialog.waitFor();
            
            if (!dialog.isInitGameAllowed()){
                return;
            }
 
            settings.setWidth(640);
            settings.setHeight(480);
            
            context = JmeContext.create(LWJGLContext.class, JmeContext.CONTEXT_WINDOW);
            context.setSettings(settings);
            context.start();
            context.waitFor();
        } catch (Exception ex){
            GameDebugManager.reportError("Error while initializing display", ex);
        }
        
        final JmeContext cx = context;
        
        // create gui
        FengGuiPass fg = new FengGuiPass("data/themes/QtCurve/QtCurve.xml", GameGUIManager.getInstance());
        cx.getPassManager().add(fg);
        fg.waitFor();
        
        // add input pass
        InputPass input = new InputPass(fg.getHandler(), true);
        cx.getPassManager().add(input);
        
        // add exit listener
        ExitListenerPass elp = new ExitListenerPass(new Callable<Object>(){
            public Object call() throws Exception {
                cx.dispose();
                return null;
            }
        });
        cx.getPassManager().add(elp);
        
        initGame(cx);
    }
    
}
