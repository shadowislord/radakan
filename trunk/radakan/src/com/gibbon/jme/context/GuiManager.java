package com.gibbon.jme.context;

import org.fenggui.Display;

public interface GuiManager {

    public void create(Display display);
    public void destroy(Display display);
    public void update(Display display, float tpf);
    
}
