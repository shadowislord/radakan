package com.radakan.game.camera;

import com.jme.renderer.Camera;
import com.radakan.game.Game;
import com.radakan.game.display.GameDisplayManager;

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
    
    public Camera getCurrentCamera(){
        return currentCam;
    }
    
}

