package com.radakan.game;

import com.radakan.game.camera.GameCameraManager;
import com.radakan.game.debug.GameSysInfoManager;
import com.radakan.game.res.GameResourceManager;
import com.radakan.game.config.GameConfigManager;
import com.radakan.game.input.GameInputManager;
import com.radakan.game.tile.GameTileManager;
import com.radakan.game.debug.GameDebugManager;
import com.radakan.game.display.GameDisplayManager;
import com.radakan.game.entity.GameEntityManager;
import com.radakan.game.gui.GameGUIManager;
import com.radakan.game.script.GameScriptManager;

public final class Game {

    private static GameDebugManager debug;
    private static GameSysInfoManager sysInfo;
    private static GameConfigManager config;
    private static GameInputManager input;
    private static GameTileManager tile;
    private static GameResourceManager resource;
    private static GameGUIManager gui;
    private static GameDisplayManager display;
    private static GameScriptManager script;
    private static GameCameraManager camera;
    private static GameEntityManager entity;
    
    public static void initializeForEditor(){
        debug = new GameDebugManager(false);
        sysInfo = new GameSysInfoManager();
    }
    
    /**
     * Creates and initializes all GameManagers.
     */
    public static void initialize(){
        sysInfo = new GameSysInfoManager();
        config = new GameConfigManager();
        debug = new GameDebugManager(config.getSettings().getBoolean("GameDebug", false));
        resource = new GameResourceManager();
        display = new GameDisplayManager();
        gui = new GameGUIManager();       
        input = new GameInputManager();
        script = new GameScriptManager();  
    }
    
    public static void main(String[] args){
        initialize();
    }
    
    public static GameDebugManager getDebugManager(){
        return debug;
    }
    
    public static GameSysInfoManager getSysInfoManager(){
        return sysInfo;
    }
    
    public static GameConfigManager getConfigManager(){
        return config;
    }
    
    public static GameInputManager getInputManager(){
        return input;
    }
    
    public static GameTileManager getTileManager(){
        return tile;
    }
    
    public static GameResourceManager getResourceManager(){
        return resource;
    }
    
    public static GameGUIManager getGUIManager(){
        return gui;
    }
    
    public static GameDisplayManager getDisplayManager(){
        return display;
    }
    
    public static GameScriptManager getScriptManager(){
        return script;
    }
    
    public static GameCameraManager getCameraManager(){
        return camera;
    }
    
    public static GameEntityManager getEntityManager(){
        return entity;
    }
}
