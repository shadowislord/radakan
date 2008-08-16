package com.radakan.gui;

import org.fenggui.Container;
import org.fenggui.layout.StaticLayout;

public abstract class UIContext extends Container {

    private boolean isBuilt = false;
    
    public UIContext(){
        setLayoutManager(new StaticLayout());
    }
    
    final void attachedInstant(){
        if (!isBuilt){
            buildGUI();
            isBuilt = true;
        }
    }
    

    public void contextAttach(){
    }
    
    public void contextDetach(){
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
