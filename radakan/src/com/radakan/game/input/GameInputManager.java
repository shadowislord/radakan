package com.radakan.game.input;

import com.jme.input.controls.GameControlManager;
import com.jme.system.GameSettings;
import com.radakan.game.Game;
import com.radakan.gui.dialogs.ControlSettingsDialog;

public class GameInputManager {

    private static final GameControlManager inputManager = new GameControlManager();
    private static IGameInputHandler handler;
    
    public static boolean showConfigDialog(){
        GameSettings settings = Game.getSettings();
        
        ControlSettingsDialog settings = new ControlSettingsDialog(inputManager, settings);
        settings.open();
        settings.waitFor();
        return settings.isAccepted();
    }
    
    public static void initControlBindings(){
        
    }
    
    public static void setInputHandler(IGameInputHandler handler){
        this.handler = handler;
    }
    
    
    
}
