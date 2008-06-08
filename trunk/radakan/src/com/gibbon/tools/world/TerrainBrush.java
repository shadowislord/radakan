package com.gibbon.tools.world;

import com.jme.bounding.BoundingSphere;
import com.jme.bounding.CollisionTreeManager;
import com.jme.math.FastMath;
import com.jme.math.Vector3f;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.List;

public class TerrainBrush {
    
    private TerrainBrush() {
    }
    
    public static void doRaiseAction(TriMesh mesh, Vector3f point, float radius, float intensity){
        EditorState state = EditorState.getState();
        
        FloatBuffer vb = mesh.getVertexBuffer();
        FloatBuffer nb = mesh.getNormalBuffer();
        
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
        
        TerrainUtil.rebuildNormalArray(vb, nb);
        
        CollisionTreeManager.getInstance().updateCollisionTree(mesh);
        mesh.updateModelBound();
        mesh.updateGeometricState(0, true);
    }
    
    private static void addInfluenced(Spatial obj, Vector3f point, float radius, List<TriMesh> targets){
        if (obj instanceof Node){
            Node n = (Node) obj;
            for (Spatial child : n.getChildren()){
                addInfluenced(child, point, radius, targets);
            }
        }else if (obj instanceof TriMesh){
            TriMesh mesh = (TriMesh)obj;
            if (mesh.getName().startsWith("TERRAIN")){
                float meshRadius = 64.0f;
                // FIXME: This part does not work regardless of what I do..
                //if (mesh.getWorldBound().getCenter().distance(point) <= (meshRadius + radius)){
                    targets.add(mesh);
                //}
            }
        }
    }
    
    public static void doMouseAction(TriMesh collided, Vector3f point){
        EditorState state = EditorState.getState();
        
        List<TriMesh> influenced = new ArrayList<TriMesh>();
        addInfluenced(World.getWorld(), point, state.brushSize, influenced);
        
        for (TriMesh mesh : influenced){
            if (state.brushType == BrushType.RAISE){
                doRaiseAction(mesh, point, (float) state.brushSize, 0.5f);
            }else if (state.brushType == BrushType.LOWER){
                doRaiseAction(mesh, point, (float) state.brushSize, -0.5f);
            }
        }
    }
    
}
