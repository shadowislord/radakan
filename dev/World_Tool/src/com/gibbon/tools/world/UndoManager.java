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
        if (list.size() > EditorSettings.getPrefs().getInt("MaxUndo", 40)){
            // remove first element if not enough elements
            list.remove(0);
        }
        list.add(action);
        System.out.println("Added "+action.getName());
    }
}
