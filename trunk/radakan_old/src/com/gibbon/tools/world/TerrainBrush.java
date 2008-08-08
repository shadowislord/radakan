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
import java.util.Collection;
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
    
    public static void doSmoothAction(TriMesh mesh, Vector3f point, float radius, float intensity){
        FloatBuffer vb = mesh.getVertexBuffer();
        Tile t = (Tile) mesh.getParent();
        vb.rewind();
        
        if (mesh.getVertexCount() <= 0)
            throw new IllegalArgumentException();
        
        float planeSize = (float)World.getWorld().getTileSize() / World.getWorld().getGridResolution();
        
        Vector3f vertex = new Vector3f();
        Vector3f sampler = new Vector3f();
        for (int i = 0; i < mesh.getVertexCount(); i++){
            // modify vertex
            vertex.set(vb.get(), vb.get(), vb.get());
            
            // convert to world space
            mesh.localToWorld(vertex, vertex);

            float dx = (vertex.x - point.x);
            float dy = (vertex.z - point.z);
            float dist = FastMath.sqrt(dx * dx + dy * dy);

            if (dist < radius) {
                // get 9 samples around point
                float blurred = 0.0f;
                float sample;
                int count = 0;
                
                sampler.set(vertex).addLocal(0.0f, 0.0f, 0.0f);
                sample = PickUtils.getTerrainHeight(sampler, null, 0);
                if (!Float.isNaN(sample)){ blurred += sample; count++; }

                sampler.set(vertex).addLocal(planeSize, 0.0f, 0.0f);
                sample = PickUtils.getTerrainHeight(sampler, null, 0);
                if (!Float.isNaN(sample)){ blurred += sample; count++; }
                
                sampler.set(vertex).addLocal(-planeSize, 0.0f, 0.0f);
                sample = PickUtils.getTerrainHeight(sampler, null, 0);
                if (!Float.isNaN(sample)){ blurred += sample; count++; }
                
                sampler.set(vertex).addLocal(0.0f, 0.0f, planeSize);
                sample = PickUtils.getTerrainHeight(sampler, null, 0);
                if (!Float.isNaN(sample)){ blurred += sample; count++; }
                
                sampler.set(vertex).addLocal(0.0f, 0.0f, -planeSize);
                sample = PickUtils.getTerrainHeight(sampler, null, 0);
                if (!Float.isNaN(sample)){ blurred += sample; count++; }
                
                sampler.set(vertex).addLocal(planeSize, 0.0f, planeSize);
                sample = PickUtils.getTerrainHeight(sampler, null, 0);
                if (!Float.isNaN(sample)){ blurred += sample; count++; }
                
                sampler.set(vertex).addLocal(planeSize, 0.0f, -planeSize);
                sample = PickUtils.getTerrainHeight(sampler, null, 0);
                if (!Float.isNaN(sample)){ blurred += sample; count++; }
                
                sampler.set(vertex).addLocal(-planeSize, 0.0f, planeSize);
                sample = PickUtils.getTerrainHeight(sampler, null, 0);
                if (!Float.isNaN(sample)){ blurred += sample; count++; }
                
                sampler.set(vertex).addLocal(-planeSize, 0.0f, -planeSize);
                sample = PickUtils.getTerrainHeight(sampler, null, 0);
                if (!Float.isNaN(sample)){ blurred += sample; count++; }
                
                t.setModified();
                
                dist = 1.0f - (dist / radius);
                vertex.y = FastMath.LERP(intensity * dist * 0.10f, vertex.y, (blurred / (float)count));
            }

            // back to model space
            mesh.worldToLocal(vertex, vertex);
            
            vb.position(vb.position() - 3);
            vb.put(vertex.x).put(vertex.y).put(vertex.z);
        }
    }
    
    public static void updateTileTerrain(Tile tile){
        TriMesh mesh = tile.getTerrain();
        
        CollisionTreeManager.getInstance().updateCollisionTree(mesh);
        mesh.updateModelBound();
        mesh.updateGeometricState(0, true);

        for (Spatial model : tile.getChildren()){
            if (model.getName().startsWith("ENTITY")){
                float h = PickUtils.getTerrainHeight(model.getWorldTranslation(), null, 0);
                model.getLocalTranslation().y = h;
            }
        }

        tile.clearModified();
    }
    
    public static void doMouseAction(int x, int y, boolean drag, boolean finish){
        EditorState state = EditorState.getState();
        
        Vector3f point = new Vector3f();
        TriMesh collided = PickUtils.findClickedObject(x, y, true, point, null);
        if (collided == null)
            return;
        
        //List<TriMesh> influenced = new ArrayList<TriMesh>();
        //WorldUtil.addInfluenced(World.getWorld(), point, state.brushSize, influenced);
        
        Collection<TriMesh> influenced = PickUtils.getCollisions(point, state.brushSize);
        
        if (!drag && !finish){
            TerrainUndoAction undoAction = new TerrainUndoAction("Undo Terrain "+state.brushType.name(),
                                                                 influenced.toArray(new TriMesh[0]));
            UndoManager.registerAction(undoAction);
        }
        
        for (TriMesh mesh : influenced){
            if (state.brushType == BrushType.RAISE){
                doRaiseAction(mesh, point, (float) state.brushSize, state.brushStrength);
            }else if (state.brushType == BrushType.LOWER){
                doRaiseAction(mesh, point, (float) state.brushSize, -state.brushStrength);
            }else if (state.brushType == BrushType.SMOOTH){
                doSmoothAction(mesh, point, (float) state.brushSize, state.brushStrength);
            }else{
                PickUtils.getTerrainHeight(point, null, 0);
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
