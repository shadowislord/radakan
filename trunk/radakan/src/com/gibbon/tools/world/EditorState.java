/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.gibbon.tools.world;


public class EditorState {

    private static final EditorState state = new EditorState();
    
    EditType editType = EditType.TERRAIN;
    
    BrushType brushType = BrushType.RAISE;
    int brushSize = 5;
    
    WorldTile[][] tiles;
    
    public static EditorState getState(){
        return state;
    }
    
}
