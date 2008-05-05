package com.gibbon.jme.context;

import com.jmex.audio.AudioSystem;

public class SoundPass extends UpdatePass {

    @Override
    public void runPass(JmeContext context) {
        AudioSystem.getSystem().update();
    }
    
    @Override
    public void initPass(JmeContext context) {
    }
    
    @Override
    public void cleanPass(JmeContext context){
        AudioSystem.getSystem().cleanup();
    }
    
}
