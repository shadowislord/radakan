package com.gibbon.tools.world;

import com.gibbon.jme.context.JmeContext;

public interface Editor {

    public void activate();
    public void deactivate();
    
    public void mouseAction(int x, int y, boolean dragged);
    
    public void render(JmeContext cx);
    public void update(float tpf);
    
    public void renderInit();
    
}
