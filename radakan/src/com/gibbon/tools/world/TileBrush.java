package com.gibbon.tools.world;

import com.jme.math.Vector3f;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;

public class TileBrush {

    public static void doMouseAction(int x, int y, boolean drag){
        Vector3f point = new Vector3f();
        TriMesh collided = PickUtils.findClickedObject(x, y, false, point);
        
        if (collided != null && collided.getName().startsWith("MARKER")){
            String[] split = collided.getName().split("_");
            int tileX = Integer.parseInt(split[1]);
            int tileY = Integer.parseInt(split[2]);

            World.getWorld().detachChild(collided);
            World.getWorld().createTile(tileX, tileY);
            World.getWorld().update();
        }
    }

    public static void previewLightmaps() {
        if (World.getWorld() != null){
            for (Spatial model : World.getWorld().getChildren()){
                if (model.getName().startsWith("GROUP")){
                    TileGroup group = (TileGroup) model;
                    group.applyLightmap();
                }
            }
        }
    }
    
}
