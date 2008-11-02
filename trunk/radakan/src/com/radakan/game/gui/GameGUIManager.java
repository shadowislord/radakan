package com.radakan.game.gui;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.pass.FengGuiPass;
import com.gibbon.jme.util.IGuiManager;
import java.util.concurrent.Callable;
import org.fenggui.Container;
import org.fenggui.Display;
import org.fenggui.decorator.background.PlainBackground;
import org.fenggui.layout.StaticLayout;
import org.fenggui.util.Color;

public class GameGUIManager extends Container implements IGuiManager {

    private FengGuiPass guiPass;
    
    public  UIContext current = null;
    private UIContext switchTo = null;

    // some stuff for fading in/out contexts
    private Container fadeContainer;
    private PlainBackground fadeBackground;
    private float fadeTimeEnd = -1f;
    private float fadeTime = 0f;
    private float fadeSpeed = 0f;
    private float defaultFadeTime = 3f;
    
    public int width, height;
   
    /**
     * Depends on:
     * - GameDisplayManager
     */
    public GameGUIManager() {
        guiPass = new FengGuiPass("data/themes/QtCurve/QtCurve.xml", this);
        JmeContext.get().getPassManager().add(guiPass);
        guiPass.waitFor(); 
    }
    
    public FengGuiPass getGUIPass(){
        return guiPass;
    }
    
    protected void setContextInstantGL(UIContext context){
        if (current != null)
            current.contextRemoved();
        
        removeAllWidgets();

        if (context != null) {
            context.setMinSize(width, height);
            context.setSizeToMinSize();

            // context is attached, do handling
            context.attachedInstant();

            // attach the new context
            addWidget(context);

            StaticLayout.center(context, this);
        }

        current = context;
        switchTo = null;
    }
    
    public void setContext(final UIContext context, final boolean fade){
        JmeContext.get().execute(new Callable<Object>(){
            public Object call(){
                if (fade){
                    if (current == null){
                        // no need to fade out first, just fade in new context
                        setContextInstantGL(context);
                        fadeTimeEnd = defaultFadeTime;
                        fadeTime = defaultFadeTime;
                        fadeSpeed = -1f;
                    }else{
                        // fade out first, update method automatically enables fade in
                        fadeSpeed = 1f;
                        fadeTimeEnd = defaultFadeTime;
                        fadeTime = 0f;
                        switchTo = context;
                        current.contextDetach();
                    }
                }else{
                    fadeSpeed = 0f;
                    switchTo = null;
                    if (current != null)
                        current.contextDetach();
                    
                    setContextInstantGL(context);
                    current.contextAttach();
                }
                
                return null;
            }
        });
    }

    /**
     * Initialize UIManager for the given FengGUI display.
     * @param display
     */
    public void create(Display display) {
        width = display.getWidth();
        height = display.getHeight();
        
        fadeBackground = new PlainBackground(new Color(0f,0f,0f,0f));
        
        fadeContainer = new Container();
        fadeContainer.setMinSize(width, height);
        fadeContainer.setSizeToMinSize();
        fadeContainer.getAppearance().add(fadeBackground);
        
        setMinSize(width, height);
        setSizeToMinSize();
        
        StaticLayout.center(this, display);
        StaticLayout.center(fadeContainer, display);
        
        display.addWidget(this);
        display.addWidget(fadeContainer);
        fadeContainer.setVisible(false);        
    }
    
    public void destroy(Display display) {
        display.removeAllWidgets();
    }

    public void update(Display display, float tpf) {
        if (current != null)
            current.update(tpf);
        
        if (fadeSpeed != 0f){
            fadeTime += (0.0333333f * 0.7f + tpf * 0.3f) * fadeSpeed;
            
            if (fadeTime > fadeTimeEnd){
                fadeTime = fadeTimeEnd;
                if (switchTo != null){
                    // now go backwards in fading in new context
                    fadeSpeed = -1f;
                    setContextInstantGL(switchTo);
                }else{
                    fadeSpeed = 0f;
                }
            }else if (fadeTime < 0f){
                fadeTime = 0f;
                fadeSpeed = 0f;
                if (current != null)
                    current.contextAttach();
                
            }
            
            fadeBackground.setColor(new Color(0f, 0f, 0f, fadeTime / fadeTimeEnd));
            if (fadeTime < 0.01f)
                fadeContainer.setVisible(false);
            else
                fadeContainer.setVisible(true);
        }else{
            fadeContainer.setVisible(false);
        }
    }

}
