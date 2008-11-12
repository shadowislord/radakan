package com.radakan.game.camera;

import com.jme.renderer.Camera;
import com.jmex.audio.AudioSystem;
import com.radakan.game.Game;
import com.radakan.game.display.GameDisplayManager;

/**
 * GameCameraManager maintains the camera view and allows cinematic playback.
 * @author Kirill
 */
public class GameCameraManager {

    private Camera defaultCam;
    private Camera currentCam;
    
    /**
     * Depends on:
     *  - GameDisplayManager
     */
    public GameCameraManager(){
        GameDisplayManager display = Game.getDisplayManager();
        defaultCam = display.getContext().getRenderer().getCamera();
        currentCam = defaultCam;
    }
    
    /**
     * @return the currently set camera
     */
    public Camera getCurrentCamera(){
        return currentCam;
    }
    
    /**
     * Animation Mode:
     *  - Disable user control
     *  - Hide the user interface
     *  - Turn display to widescreen mode
     *  - Stop background music and environmental sounds
     * 
     * @param enable
     */
    public void setAnimationMode(boolean enable){
        if (enable){
            AudioSystem.getSystem().getMusicQueue().pause();
            currentCam.setViewPort(0, 0, 1, 1);
        }else{
            AudioSystem.getSystem().getMusicQueue().play();
            currentCam.setViewPort(0, 0, 1, 1);
        }
        
        Game.getGUIManager().setEnabled(enable);
        Game.getInputManager().setEnabled(enable);
    }
    
}

