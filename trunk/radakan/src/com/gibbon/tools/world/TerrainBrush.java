package com.gibbon.tools.world;

import com.jme.bounding.BoundingSphere;
import com.jme.bounding.CollisionTreeManager;
import com.jme.math.FastMath;
import com.jme.math.Vector3f;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import com.jme.util.geom.BufferUtils;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.List;

public class TerrainBrush {
    
    private TerrainBrush() {
    }
    
    public static void doRaiseAction(TriMesh mesh, Vector3f point, float radius, float intensity){
        FloatBuffer vb = mesh.getVertexBuffer();
        Tile t = (Tile) mesh.getParent();
        vb.rewind();
        
        if (mesh.getVertexCount() <= 0)
            throw new IllegalArgumentException();
        
        Vector3f vertex = new Vector3f();
        for (int i = 0; i < mesh.getVertexCount(); i++){
            // modify vertex
            vertex.set(vb.get(), vb.get(), vb.get());
            
            // convert to world space
            mesh.localToWorld(vertex, vertex);

            float dx = (vertex.x - point.x);
            float dy = (vertex.z - point.z);
            float dist = FastMath.sqrt(dx * dx + dy * dy);

            if (dist < radius) {
                dist = 1.0f - (dist / radius);
                vertex.addLocal(0f, intensity * dist, 0f);
                t.setModified();
            }

            // back to model space
            mesh.worldToLocal(vertex, vertex);
            
            vb.position(vb.position() - 3);
            vb.put(vertex.x).put(vertex.y).put(vertex.z);
        }
    }
    
    public static void takeSample(FloatBuffer me, FloatBuffer left, FloatBuffer right, FloatBuffer up, FloatBuffer down, int size, int x, int y, Vector3f store){
        if (x < 0){
            x += size;
            BufferUtils.populateFromBuffer(store, left, y*size+x);
        }else if (x >= size){
            x -= size;
            BufferUtils.populateFromBuffer(store, right, y*size+x);
        }
    }
    
    public static void doSmoothAction(TriMesh mesh, Vector3f point, float radius, float intensity){
        FloatBuffer vb = mesh.getVertexBuffer();
        vb.rewind();
        
        if (mesh.getVertexCount() <= 0)
            throw new IllegalArgumentException();
        
        Vector3f vertex = new Vector3f();
        for (int i = 0; i < mesh.getVertexCount(); i++){
            // modify vertex
            vertex.set(vb.get(), vb.get(), vb.get());
            
            // convert to world space
            mesh.localToWorld(vertex, vertex);

            float dx = (vertex.x - point.x);
            float dy = (vertex.z - point.z);
            float dist = FastMath.sqrt(dx * dx + dy * dy);

            if (dist < radius) {
                dist = 1.0f - (dist / radius);
                vertex.addLocal(0f, intensity * dist, 0f);
            }

            // back to model space
            mesh.worldToLocal(vertex, vertex);
            
            vb.position(vb.position() - 3);
            vb.put(vertex.x).put(vertex.y).put(vertex.z);
        }
    }
    
    public static void doMouseAction(TriMesh collided, Vector3f point){
        EditorState state = EditorState.getState();
        
        List<TriMesh> influenced = new ArrayList<TriMesh>();
        WorldUtil.addInfluenced(World.getWorld(), point, state.brushSize, influenced);
        
        for (TriMesh mesh : influenced){
            if (state.brushType == BrushType.RAISE){
                doRaiseAction(mesh, point, (float) state.brushSize, state.brushStrength);
            }else if (state.brushType == BrushType.LOWER){
                doRaiseAction(mesh, point, (float) state.brushSize, -state.brushStrength);
            }
        }
        
        for (TriMesh mesh : influenced){
            Tile t = (Tile) mesh.getParent();
            if (t.isModified()){
                TerrainUtil.rebuildNormalArray(t);
            }
        }
    }
    
}
