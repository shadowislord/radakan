package com.gibbon.tools.world;

import com.gibbon.jme.context.JmeContext;
import com.jme.bounding.CollisionTreeManager;
import com.jme.math.FastMath;
import com.jme.math.Ray;
import com.jme.math.Vector2f;
import com.jme.math.Vector3f;
import com.jme.renderer.Renderer;
import com.jme.scene.TriMesh;
import java.nio.FloatBuffer;

public class TerrainBrush {
    
    private TerrainBrush() {
    }
    
    public static void recalcNormals(TriMesh mesh, int size){
        FloatBuffer vb = mesh.getVertexBuffer();
        FloatBuffer nb = mesh.getNormalBuffer();
    }
    
    public static void doRaiseAction(TriMesh mesh, Vector3f point, float radius, float intensity){
        FloatBuffer vb = mesh.getVertexBuffer();
        FloatBuffer nb = mesh.getNormalBuffer();
        
        vb.rewind();
        
        Vector3f vertex = new Vector3f();
        for (int y = 0; y < 64; y++){
            for (int x = 0; x < 64; x++){
                // modify vertex
                vertex.set(vb.get(), vb.get(), vb.get());

                float dx = (vertex.x - point.x);
                float dy = (vertex.z - point.z);
                float dist = FastMath.sqrt(dx*dx + dy*dy);
                
                if (dist < radius){
                    dist = 1.0f - (dist / radius);
                    vertex.addLocal(0f, intensity * dist, 0f);
                }

                vb.position(vb.position()-3);
                vb.put(vertex.x).put(vertex.y).put(vertex.z);
            }
        }
        
        WorldUtil.rebuildNormalArray(vb, nb);
        
        CollisionTreeManager.getInstance().updateCollisionTree(mesh);
    }
    
    public static void doMouseAction(int x, int y){
        Renderer r = JmeContext.get().getRenderer();
        
        EditorState state = EditorState.getState();
        final int nx = x;
        final int ny = r.getHeight() - y;
        
        Vector3f worldCoords = r.getCamera().getWorldCoordinates(new Vector2f((float)nx, (float)ny), 1.0f);
        Vector3f camLoc = r.getCamera().getLocation().clone();
        Vector3f rayDir = worldCoords.subtract(camLoc).normalizeLocal();
        
        Ray mouseRay = new Ray(camLoc, rayDir);

        TriMesh collided = (TriMesh) WorldUtil.calculatePick(state.terrain, mouseRay, worldCoords);
        if (collided == null){
            return;
        }
        
        if (state.editType == EditType.TERRAIN){
            if (state.brushType == BrushType.RAISE){
                doRaiseAction(collided, worldCoords, (float) state.brushSize, 0.5f);
            }else if (state.brushType == BrushType.LOWER){
                doRaiseAction(collided, worldCoords, (float) state.brushSize, -0.5f);
            }
        }
    }

}
