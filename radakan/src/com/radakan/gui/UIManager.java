package com.radakan.gui;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.util.IGuiManager;
import com.radakan.util.ErrorHandler;
import java.io.IOException;
import java.util.concurrent.Callable;
import org.fenggui.Container;
import org.fenggui.Display;
import org.fenggui.FengGUI;
import org.fenggui.layout.StaticLayout;
import org.fenggui.theme.XMLTheme;
import org.fenggui.theme.xml.IXMLStreamableException;

public class UIManager extends Container implements IGuiManager {

    private static UIManager singleton;
    private static UIContext current = null;
    
    public static int width, height;
    
    public static UIManager getInstance(){
        if (singleton == null){
            singleton = new UIManager();
        }
        return singleton;
    }
    
    public static void setXMLTheme(final String name){
        JmeContext.get().execute(new Callable<Object>(){
            public Object call(){
                try {
                    FengGUI.setTheme(new XMLTheme(name));
                } catch (IOException ex) {
                    ErrorHandler.reportError("Error while reading theme file QtCurve.xml", ex);
                } catch (IXMLStreamableException ex) {
                    ErrorHandler.reportError("Error while parsing XML theme file", ex);
                }
                
                return null;
            }
        });
    }
    
    public static void setContext(final UIContext context, boolean fade){
        JmeContext.get().execute(new Callable<Object>(){
            public Object call(){
                // detach the old context
                getInstance().removeAllWidgets();

                context.setMinSize(width, height);
                context.setSizeToMinSize();
                
                // build the gui
                context.buildGUI();
                
                // attach the new context
                getInstance().addWidget(context);
                
                StaticLayout.center(context, getInstance());

                current = context;
                
                return null;
            }
        });
    }

    public void create(Display display) {
        display.addWidget(singleton);
        
        width = display.getWidth();
        height = display.getHeight();
        
        setMinSize(width, height);
        setSizeToMinSize();
        
        StaticLayout.center(this, display);
    }

    public void destroy(Display display) {
        display.removeAllWidgets();
    }

    public void update(Display display, float tpf) {
        if (current != null)
            current.update(tpf);
    }
    
}
