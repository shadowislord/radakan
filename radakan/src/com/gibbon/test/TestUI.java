package com.gibbon.test;

import com.gibbon.jme.context.ExitListenerPass;
import com.gibbon.jme.context.FengGuiPass;
import com.gibbon.jme.context.GuiManager;
import com.gibbon.jme.context.InputPass;
import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.gibbon.radakan.config.ConfigFrame;
import com.gibbon.radakan.error.ErrorReporter;
import com.jme.system.GameSettings;
import com.jme.system.PreferencesGameSettings;
import java.util.concurrent.Callable;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.prefs.Preferences;
import org.fenggui.Display;
import org.fenggui.Label;
import org.fenggui.composites.Window;
import org.fenggui.menu.MenuBar;
import org.fenggui.menu.MenuItem;
import org.fenggui.menu.Menu;
import org.fenggui.event.IMenuItemPressedListener;
import org.fenggui.event.MenuItemPressedEvent;
import org.fenggui.util.Point;

public class TestUI {

    public static void main(String[] args){
        //Logger.getLogger("").setLevel(Level.WARNING);
        
        GameSettings settings
                = new PreferencesGameSettings(Preferences.userRoot().node("test-jmecontext"));
        
        settings.set("title", "Test User Interface");
        
        JmeContext context = null;
        try{
            ConfigFrame g = new ConfigFrame(settings);
            g.setVisible(true);
            g.waitFor();

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
            public void create(Display display) {
                Window test = new Window(true, false, false);
                test.setLayoutManager(new org.fenggui.layout.RowLayout(false));
                
                test.setTitle("This is a FengGUI window!!!");
                
                Label label = new Label("And this is a FengGUI label!!");
                test.getContentContainer().addWidget(label);
                
                test.pack();
                
                test.setPosition(new Point(display.getWidth() / 2 - test.getWidth() / 2,
                                           display.getHeight() / 2 - test.getHeight() / 2));
                
                Menu menuFile = new Menu();
                MenuItem itemFileExit = new MenuItem("Exit");
                itemFileExit.addMenuItemPressedListener(
                        new IMenuItemPressedListener(){
                            public void menuItemPressed(MenuItemPressedEvent event){
                                cx.dispose();
                                //System.exit(0);
                            }
                        }
                );
                        
                
                menuFile.addItem(itemFileExit);
                
                MenuBar menuBar = new MenuBar();
                menuBar.registerSubMenu(menuFile, "File");
                
                display.addWidget(menuBar);
                
                menuBar.updateMinSize(); // we have not layouted anything yet...
		menuBar.setX(0);
		menuBar.setY(display.getHeight() - menuBar.getMinHeight());
		menuBar.setSize(display.getWidth(), menuBar.getMinHeight());
		menuBar.setShrinkable(false);
                
                display.displayPopUp(test);
                display.layout();
            }

            public void destroy(Display display) {
            }

            public void update(Display display, float tpf) {
            }

        };
        
        // add gui pass
        FengGuiPass ui = new FengGuiPass(manager);
        cx.getPassManager().add(ui);
    }
    
}
