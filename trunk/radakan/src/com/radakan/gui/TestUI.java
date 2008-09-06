package com.radakan.gui;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.gibbon.jme.pass.ExitListenerPass;
import com.gibbon.jme.pass.FengGuiPass;
import com.gibbon.jme.pass.InputPass;
import com.jme.input.Mouse;
import com.jme.system.GameSettings;
import com.jme.system.PreferencesGameSettings;
import com.radakan.game.Game;
import com.radakan.gui.dialogs.GameSettingsDialog;
import com.radakan.util.ErrorHandler;
import java.io.IOException;
import java.util.concurrent.Callable;
import java.util.prefs.Preferences;
import org.lwjgl.input.Cursor;

public class TestUI {

    public static void initGame(JmeContext cx){
        //com/radakan/data/themes/QtCurve.xml
        //Binding.getInstance().setUseClassLoader(true);
       
        LoadingTask test = new LoadingTask() {
            public void doInBackground() {
                System.out.println("doInBackground: "+Thread.currentThread().getName());
                // simulate work
                while (getProgress() <= 100) {
                    progress((int)(Math.random() * 10.0));
                    progress(getProgress() + "%");
                    try {
                        Thread.sleep(200);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
            public void done(){
                UIManager.setContext(new MainMenu(), true);
                //System.out.println("done: "+Thread.currentThread().getName());
            }
        };
        
        UIContext ui = new LoadScreen(test);
        //UIContext ui = new MainMenu();
        UIManager.setContext(ui, true);
    }
    
    public static void main(String[] args) throws IOException{
        GameSettings settings
                = new PreferencesGameSettings(Preferences.userRoot().node("test"));
        
        settings.set("title", "Test Main Menu");
        
        Game.setupDefaultLocators();
        
        JmeContext context = null;
        try{
            //ConfigFrame g = new ConfigFrame(settings);
            //g.setVisible(true);
            //g.waitFor();

            GameSettingsDialog dialog = new GameSettingsDialog(settings);
            dialog.open();
            dialog.waitFor();
            
            if (!dialog.isInitGameAllowed()){
                return;
            }
            
//            settings.setWidth(320);
//            settings.setHeight(240);
            
            context = JmeContext.create(LWJGLContext.class, JmeContext.CONTEXT_WINDOW);
            context.setSettings(settings);
            context.start();
            context.waitFor();
        } catch (Exception ex){
            ErrorHandler.reportError("Error while initializing display", ex);
        }
        
        final JmeContext cx = context;
        
        // create gui
        FengGuiPass fg = new FengGuiPass("data/themes/QtCurve/QtCurve.xml", UIManager.getInstance());
        cx.getPassManager().add(fg);
        fg.waitFor();
        
        // add input pass
        InputPass input = new InputPass(fg.getHandler(), true);
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
