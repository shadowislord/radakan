/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.gibbon.tools.world;

import com.jme.bounding.BoundingSphere;


public class EditorState {

    private static final EditorState state = new EditorState();
    
    EditType editType = EditType.TERRAIN;

    BrushType brushType = BrushType.RAISE;
    int brushSize = 5;
    BoundingSphere sphere = new BoundingSphere();
    
    public static EditorState getState(){
        return state;
    }
    
}
