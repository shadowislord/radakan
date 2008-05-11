package com.gibbon.test;

import com.gibbon.jme.context.ExitListenerPass;
import com.gibbon.jme.context.FengGuiPass;
import com.gibbon.jme.context.GuiManager;
import com.gibbon.jme.context.InputPass;
import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.gibbon.radakan.cinematic.Cinematic;
import com.gibbon.radakan.config.ConfigFrame;
import com.gibbon.radakan.error.ErrorReporter;

import com.jme.system.GameSettings;
import com.jme.system.PreferencesGameSettings;

import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.concurrent.Callable;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.prefs.Preferences;

import org.fenggui.Display;
import org.fenggui.FengGUI;
import org.fenggui.layout.StaticLayout;
import org.fenggui.theme.XMLTheme;
import org.fenggui.theme.xml.IXMLStreamableException;

public class TestUI {

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
        
        // create gui
        GuiManager manager = new GuiManager(){
            
            private Cinematic c;
            
            public void create(Display display) {
                try {
                    FengGUI.setTheme(new XMLTheme("data/themes/QtCurve.xml"));
                } catch (IOException ex) {
                    ErrorReporter.reportError("Error while reading theme file QtCurve.xml", ex);
                } catch (IXMLStreamableException ex) {
                    ErrorReporter.reportError("Error while parsing XML theme file", ex);
                }
                
//                try {
                MainMenu menu = new MainMenu(display.getWidth(), display.getHeight());
                display.addWidget(menu);
                
                StaticLayout.center(menu, display);
//                    URL url = new File("E:\\RADAKAN\\data\\cinematic\\final.mpg").toURI().toURL();
//                    c = new Cinematic(url, display.getWidth(), display.getHeight());
//                    display.addWidget(c);
//
//                    StaticLayout.center(c, display);
//                    
//                    c.realize();
//                    c.play();
                    
                    display.layout();
//                } catch (MalformedURLException ex) {
//                    Logger.getLogger(TestUI.class.getName()).log(Level.SEVERE, null, ex);
//                }
            }

            public void destroy(Display display) {
                c.dispose();
                System.out.println("Cinematic disposed");
            }

            public void update(Display display, float tpf) {
            }

        };
        
        // add gui pass
        FengGuiPass ui = new FengGuiPass(manager);
        cx.getPassManager().add(ui);
    }
    
}
