package com.radakan.game.gui;

import com.gibbon.jme.context.JmeContext;

/**
 * An exit screen closes up the game display when it becomes active.
 * 
 * @author Kirill Vainer
 */
public class DoExitScreen extends UIContext {

    public void buildGUI() {
    }

    @Override
    public void update(float tpf) {
    }
    
    @Override
    public void contextAttach(){
        JmeContext.get().dispose();
    }

}
