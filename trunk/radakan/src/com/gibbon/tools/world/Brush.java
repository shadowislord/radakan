package com.gibbon.tools.world;

import com.gibbon.jme.context.JmeContext;
import com.jme.math.Ray;
import com.jme.math.Vector2f;
import com.jme.math.Vector3f;
import com.jme.renderer.Renderer;
import com.jme.scene.TriMesh;

public class Brush {

    public static void doMouseAction(int x, int y){
        Renderer r = JmeContext.get().getRenderer();
        
        EditorState state = EditorState.getState();
        final int nx = x;
        final int ny = r.getHeight() - y;
        
        Vector3f worldCoords = r.getCamera().getWorldCoordinates(new Vector2f((float)nx, (float)ny), 1.0f);
        Vector3f camLoc = r.getCamera().getLocation().clone();
        Vector3f rayDir = worldCoords.subtract(camLoc).normalizeLocal();
        
        Ray mouseRay = new Ray(camLoc, rayDir);

        TriMesh collided = (TriMesh) WorldUtil.calculatePick(World.getWorld(), mouseRay, worldCoords);
        if (collided == null){
            return;
        }

        if (state.editType == EditType.TILE){
            TileBrush.doMouseAction(collided, worldCoords);
        }else if (state.editType == EditType.TERRAIN){
            TerrainBrush.doMouseAction(collided, worldCoords);
        }else if (state.editType == EditType.TEXTURE){
            TextureBrush.doMouseAction(collided, worldCoords);
        }
    }
    
    
}
