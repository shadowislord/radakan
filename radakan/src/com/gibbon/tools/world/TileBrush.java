package com.gibbon.tools.world;

import com.jme.math.Vector3f;
import com.jme.scene.TriMesh;

public class TileBrush {

    public static void doMouseAction(TriMesh collided, Vector3f point){
        if (collided.getName().startsWith("MARKER")){
            String[] split = collided.getName().split("_");
            int tileX = Integer.parseInt(split[1]);
            int tileY = Integer.parseInt(split[2]);

            World.getWorld().detachChild(collided);
            World.getWorld().createTile(tileX, tileY);
            World.getWorld().update();
        }
    }
    
}
