package com.gibbon.tools.world;

import java.util.ArrayList;
import java.util.List;

import com.radakan.game.Game;
import com.radakan.game.config.GameConfigManager;

public class UndoManager {

    private static final List<UndoAction> list = new ArrayList<UndoAction>();
    private static final List<UndoAction> redoList = new ArrayList<UndoAction>();
    
    public static void doUndo(){
    	GameConfigManager configMan = Game.getConfigManager();
        int maxRedo = configMan.getSettings().getInt("MaxRedo", 5);
        
        if (list.size() != 0){
            UndoAction action = list.remove(list.size()-1);
            UndoAction redoAction = action.restore(true);
            System.out.println("Undid: "+action.getName());
            
            if (redoList.size() > maxRedo){
                redoList.remove(0);
            }
            redoList.add(redoAction);
        }
    }
    
    public static void doRedo(){
        if (redoList.size() != 0){
            UndoAction redoAction = redoList.remove(redoList.size()-1);
            redoAction.restore(false);
        }
    }
    
    public static void registerAction(UndoAction action){
    	GameConfigManager configMan = Game.getConfigManager();
        if (list.size() > configMan.getSettings().getInt("MaxUndo", 40)){
            // remove first element if not enough elements
            list.remove(0);
        }
        list.add(action);
        System.out.println("Added "+action.getName());
    }

    public static UndoAction getLastAction(){
        return list.get(list.size()-1);
    }
}
