package com.radakan.gui;

import org.fenggui.Container;
import org.fenggui.layout.StaticLayout;

public abstract class UIContext extends Container {

    public UIContext(){
        setLayoutManager(new StaticLayout());
    }
    
    public abstract void buildGUI();
    
    public abstract void update(float tpf);
    
}
