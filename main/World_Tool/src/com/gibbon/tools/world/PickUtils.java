package com.gibbon.tools.world;

import com.gibbon.jme.context.JmeContext;
import com.jme.bounding.BoundingBox;
import com.jme.intersection.PickData;
import com.jme.intersection.TrianglePickResults;
import com.jme.math.Ray;
import com.jme.math.Triangle;
import com.jme.math.Vector2f;
import com.jme.math.Vector3f;
import com.jme.renderer.Renderer;
import com.jme.scene.Geometry;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import com.jme.util.geom.BufferUtils;
import com.jmex.terrain.TerrainBlock;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

public class PickUtils {

    private static final Vector3f bicentric = new Vector3f();
    private static final Vector3f clickedLoc = new Vector3f();
    private static final Vector3f normal = new Vector3f();
    private static PickData lastPickData;
    private static boolean success = false;
    
    private static boolean terrainOnly = false;
    private static boolean calculateNormal = false;
    
    private static final Vector3f[] normals = new Vector3f[] { new Vector3f(), new Vector3f(), new Vector3f() };
    
    private static TrianglePickResults camResults = new TrianglePickResults() {
        @Override
        public void processPick() {
            if (getNumber() > 0) {
                // get the state of the triangle picking
                PickData pData = null;
                if (terrainOnly){
                    for (int i = 0; i < getNumber(); i++){
                        if (getPickData(i).getTargetMesh().getName().startsWith("TERRAIN")){
                            pData = getPickData(i);
                            break;
                        }
                    }
                }else{
                    pData = getPickData(0);
                }
                
                if (pData == null)
                    return;

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
                    if (calculateNormal){
                        FloatBuffer normalBuf = mesh.getNormalBuffer();
                        IntBuffer indexBuf = mesh.getIndexBuffer();
                        
                        pData.getRay().intersectWhere(vec[0], vec[1], vec[2], bicentric);
                        
                        BufferUtils.populateFromBuffer(normals[0], normalBuf, indexBuf.get(triIndex * 3));
                        BufferUtils.populateFromBuffer(normals[1], normalBuf, indexBuf.get(triIndex * 3 + 1));
                        BufferUtils.populateFromBuffer(normals[2], normalBuf, indexBuf.get(triIndex * 3 + 2));
                        
//                        normals[1].interpolate(normals[0], normals[1], bicentric.y);
//                        normals[2].interpolate(normals[0], normals[2], bicentric.y);
//                        normal.set(normals[1]).interpolate(normals[2], bicentric.z);
//                        
//                        normal.normalizeLocal();
                        normal.set(normals[0]).normalizeLocal();
                    }
                    
                    lastPickData = pData;
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
    
//    public static void addInfluenced(Spatial obj, Vector3f point, float radius, List<TriMesh> targets){
//        if (obj instanceof Node){
//            Node n = (Node) obj;
//            for (Spatial child : n.getChildren()){
//                addInfluenced(child, point, radius, targets);
//            }
//        }else if (obj instanceof TriMesh){
//            TriMesh mesh = (TriMesh)obj;
//            if (mesh.getName().startsWith("TERRAIN")){
//                float meshRadius = 64.0f;
//                // FIXME: This part does not work regardless of what I do..
//                //if (mesh.getWorldBound().getCenter().distance(point) <= (meshRadius + radius)){
//                    targets.add(mesh);
//                //}
//            }
//        }
//    }
    
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
    
    public static float getTerrainHeight(Spatial scene, Vector3f point, Vector3f normalStore){
        camResults.clear();
        PickUtils.terrainOnly = true;
        Ray r = new Ray(new Vector3f(point), new Vector3f(0f, 1f, 0f));
        r.getOrigin().y = -5.0f;
        r.getDirection().normalizeLocal();
        
        // if given a vector to store a normal in, 
        // means we need to calculate the normal;
        calculateNormal = normalStore != null;
        scene.calculatePick(r, camResults);
        
        if (success){
            if (calculateNormal){
                normalStore.set(normal);
            }
            
            return clickedLoc.y;
        }
        if (calculateNormal){
            normalStore.set(Vector3f.ZERO);
        }
        return Float.NaN;
    }
    
    public static Geometry calculatePick(Spatial scene, Ray r, Vector3f point, boolean terrainOnly){
        camResults.clear();
        PickUtils.terrainOnly = terrainOnly;
        scene.calculatePick(r, camResults);
        
        if (success){
            point.set(clickedLoc);
            return lastPickData.getTargetMesh();
        }
        return null; 
    }
    
    public static TriMesh findClickedObject(int x, int y, boolean terrainOnly, Vector3f point){
        Renderer r = JmeContext.get().getRenderer();
        
        final int nx = x;
        final int ny = r.getHeight() - y;
        
        Vector3f worldCoords = r.getCamera().getWorldCoordinates(new Vector2f((float)nx, (float)ny), 1.0f);
        Vector3f camLoc = r.getCamera().getLocation().clone();
        Vector3f rayDir = worldCoords.subtract(camLoc).normalizeLocal();
        Ray mouseRay = new Ray(camLoc, rayDir);
        
        return (TriMesh) PickUtils.calculatePick(World.getWorld(), mouseRay, point, terrainOnly);
    }
    
}
