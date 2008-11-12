package com.radakan.game.gui;

import org.fenggui.Container;
import org.fenggui.layout.StaticLayout;

/**
 * The UIContext represents a "Screen" of the user interface.
 * For example, it can represent a loading screen, main menu, start screen,
 * and the in-game user interface. To make the "Screen" appear,
 * use the GameGUIManager.setContext() method.
 * 
 * @see GameGUIManager
 * 
 * @author Kirill Vainer
 */
public abstract class UIContext extends Container {

    private boolean isBuilt = false;
    
    /**
     * Create a new UIContext. Subclasses should do their initialization
     * in the buildGUI() method as the constructor is not gauranteed to be called
     * in the GL thread.
     */
    public UIContext(){
        setLayoutManager(new StaticLayout());
    }
    
    final void attachedInstant(){
        if (!isBuilt){
            buildGUI();
            isBuilt = true;
        }
    }

    /**
     * Called when the context has became active.
     */
    public void contextAttach(){
    }
    
    public void contextDetach(){
    }
    
    public void contextRemoved(){
    }
    
    /**
     * Called to build the gui inside the GL thread.
     * This will only be called once per UIContext object.
     */
    public abstract void buildGUI();
    
    /**
     * Update GUI.
     * @param tpf Time per frame
     */
    public abstract void update(float tpf);
    
}
