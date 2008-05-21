package com.gibbon.radakan.ui;

import org.fenggui.Container;
import org.fenggui.IWidget;
import org.fenggui.layout.StaticLayout;

public abstract class UIContext extends Container {

    public UIContext(){
        setLayoutManager(new StaticLayout());
    }
    
    public abstract void buildGUI();
    
}
