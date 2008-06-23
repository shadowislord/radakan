package com.gibbon.tools.world;

import com.jme.intersection.PickData;
import com.jme.intersection.TrianglePickResults;
import com.jme.math.Ray;
import com.jme.math.Vector3f;
import com.jme.scene.Geometry;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import java.util.List;

public class WorldUtil {

    private static final Vector3f clickedLoc = new Vector3f();
    private static boolean success = false;
    
    private static TrianglePickResults camResults = new TrianglePickResults() {
        @Override
        public void processPick() {
            if (getNumber() > 0) {
                // get the state of the triangle picking
                PickData pData = getPickData(0);

                // triangles that collided with the ray (e.g under the player)
                List<Integer> tris = pData.getTargetTris();

                // mesh with which the collision was made
                TriMesh mesh = (TriMesh) pData.getTargetMesh();

                if (tris.isEmpty()){
                    success = false;
                    return;
                }
                
                // get the first triangle under the player
                int triIndex = (tris.get(0)).intValue();
                Vector3f[] vec = new Vector3f[3];
                mesh.getTriangle(triIndex, vec);
                
                // convert triangle to world space
                for (int x = 0; x < vec.length; x++) {
                    vec[x].multLocal(mesh.getWorldScale());
                    mesh.getWorldRotation().mult(vec[x], vec[x]);
                    vec[x].addLocal(mesh.getWorldTranslation());
                }

                // find WHERE the triangle was picked.. 
                if (!pData.getRay().intersectWhere(vec[0], vec[1], vec[2], clickedLoc)){
                    success = false;
                }else{
                    success = true;
                }
            }else{
                success = false;
            }
        }
    };
    
    static {
        camResults.setCheckDistance(true);
    }
    
    public static void addInfluenced(Spatial obj, Vector3f point, float radius, List<TriMesh> targets){
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
    
    public static Geometry calculatePick(Spatial scene, Ray r, Vector3f point){
        camResults.clear();
        scene.calculatePick(r, camResults);
        
        if (success){
            point.set(clickedLoc);
            return camResults.getPickData(0).getTargetMesh();
        }
        return null;
    }
    
}
