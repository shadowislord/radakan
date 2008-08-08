package com.gibbon.jme.pass;

import com.gibbon.jme.context.JmeContext;
import com.jmex.audio.AudioSystem;

/**
 * Pass for updating jMe's sound and audio system
 * 
 * @author Momoko_Fan
 */
public class SoundPass extends UpdatePass {

    /**
     * Updates OpenAL, playing sound sources, music and environmental sounds as defined in AudioSystem.
     * @param context
     */
    @Override
    public void runPass(JmeContext context) {
        AudioSystem.getSystem().update();
    }
    
    /**
     * Initializes OpenAL.
     */
    @Override
    public void initPass(JmeContext context) {
        AudioSystem.getSystem();
    }
    
    /**
     * Deallocates all tracks and destroys the OpenAL context.
     * @param context
     */
    @Override
    public void cleanPass(JmeContext context){
        AudioSystem.getSystem().cleanup();
    }
    
}
