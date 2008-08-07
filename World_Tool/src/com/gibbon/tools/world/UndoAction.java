package com.gibbon.tools.world;

public interface UndoAction {

    public UndoAction restore(boolean returnRedoAction);

    public String getName();
    
}
