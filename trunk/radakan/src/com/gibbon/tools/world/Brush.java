package com.gibbon.tools.world;

import com.gibbon.jme.context.JmeContext;
import com.jme.bounding.BoundingBox;
import com.jme.intersection.BoundingCollisionResults;
import com.jme.intersection.CollisionResults;
import com.jme.math.Ray;
import com.jme.math.Vector2f;
import com.jme.math.Vector3f;
import com.jme.renderer.Renderer;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

public class Brush {


    public static void findBrushCollisions(Spatial spatial, BoundingBox brush, Collection<TriMesh> collided) {
        if (spatial.getWorldBound() != null && spatial.isCollidable()) {
            if (brush.intersects(spatial.getWorldBound())) {
                if (spatial instanceof Node){
                    // further checking needed.
                    Node n = (Node) spatial;
                    for (int i = 0; i < n.getQuantity(); i++) {
                        findBrushCollisions(n.getChild(i), brush, collided);
                    }
                }else if (spatial instanceof TriMesh && spatial.getName().startsWith("TERRAIN")){
                    // add to results
                    collided.add((TriMesh)spatial);
                }
            }
        }
    }
    
    public static Collection<TriMesh> getCollisions(Vector3f point, float radius){
        BoundingBox box = (BoundingBox) World.getWorld().getWorldBound();
        BoundingBox brush = new BoundingBox(point, radius * 1.5f, box.zExtent, radius * 1.5f);
        
        List<TriMesh> collided = new ArrayList<TriMesh>();
        findBrushCollisions(World.getWorld(), brush, collided);
        return collided;
    }
    
    public static void doMouseAction(int x, int y, boolean drag){
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
        }else if (state.editType == EditType.ENTITY){
            EntityBrush.doMouseAction(collided, worldCoords, drag);
        }
    }
    
    
}
