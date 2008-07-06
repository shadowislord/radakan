package com.gibbon.tools.world;

import java.util.ArrayList;
import java.util.List;

public class UndoManager {

    private static final List<UndoAction> list = new ArrayList<UndoAction>();
    private static final List<UndoAction> redoList = new ArrayList<UndoAction>();
    
    public static void doUndo(){
        int maxRedo = EditorSettings.getPrefs().getInt("MaxRedo", 5);
        
        if (list.size() != 0){
            UndoAction action = list.remove(list.size()-1);
            if (redoList.size() < maxRedo){
                redoList.add(action.restore(true));
            }
        }
    }
    
    public static void doRedo(){
        if (redoList.size() != 0){
            UndoAction redoAction = redoList.remove(list.size()-1);
            redoAction.restore(false);
        }
    }
    
    public static void registerAction(UndoAction action){
        list.add(action);
    }
}
