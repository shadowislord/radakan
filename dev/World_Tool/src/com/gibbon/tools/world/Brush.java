package com.gibbon.tools.world;

public class Brush {

    public static void doMouseAction(int x, int y, boolean drag, boolean finish){
        EditorState state = EditorState.getState();
        
        if (state.editType == EditType.TILE){
            TileBrush.doMouseAction(x, y, drag, finish);
        }else if (state.editType == EditType.TERRAIN){
            TerrainBrush.doMouseAction(x, y, drag, finish);
        }else if (state.editType == EditType.TEXTURE){
            TextureBrush.doMouseAction(x, y, drag, finish);
        }else if (state.editType == EditType.ENTITY){
            EntityBrush.doMouseAction(x, y, drag, finish);
        }
    }
    
    
}
