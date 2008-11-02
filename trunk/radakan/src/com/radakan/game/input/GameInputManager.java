package com.radakan.game.input;

import com.gibbon.jme.pass.ExitListenerPass;
import com.gibbon.jme.pass.InputPass;
import com.jme.input.controls.GameControlManager;
import com.jme.system.GameSettings;
import com.radakan.game.Game;
import com.radakan.game.gui.GameGUIManager;
import com.radakan.gui.dialogs.ControlSettingsDialog;

import com.gibbon.jme.context.*;
import java.util.logging.Level;
import java.util.logging.Logger;

public class GameInputManager {

    private GameControlManager inputManager = new GameControlManager();
    private IGameInputHandler handler;
    
    /**
     * Depends on:
     *  - GameGUIManager
     */
    public GameInputManager(){
        GameGUIManager gui = Game.getGUIManager();
        
        InputPass input = new InputPass(gui.getGUIPass().getHandler(), true);
        JmeContext.get().getPassManager().add(input);

        ExitListenerPass exitListener = new ExitListenerPass();
        JmeContext.get().getPassManager().add(exitListener);
    }
    
    public boolean showConfigDialog(){
        GameSettings settings = Game.getConfigManager().getSettings();
        
        ControlSettingsDialog dialog = new ControlSettingsDialog(inputManager, settings);
        dialog.open();
        try {
            dialog.waitFor();
        } catch (InterruptedException ex) {
            Game.getDebugManager().reportError("Interrupted while waiting for dialog to close", ex);
        }
        return dialog.isAccepted();
    }
    
    public void setInputHandler(IGameInputHandler handler){
        this.handler = handler;
    }
    
    
    
}
