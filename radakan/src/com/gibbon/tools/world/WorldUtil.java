package com.gibbon.tools.world;

import com.jme.intersection.PickData;
import com.jme.intersection.TrianglePickResults;
import com.jme.math.FastMath;
import com.jme.math.Ray;
import com.jme.math.Vector2f;
import com.jme.math.Vector3f;
import com.jme.scene.Geometry;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import com.jme.util.geom.BufferUtils;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.List;

public class WorldUtil {

    private static FloatBuffer writeVertexArray(int width, int height, Vector3f scale){
        FloatBuffer store = BufferUtils.createFloatBuffer(width*height*3);

        // going through Y/Z first, scanlines are horizontal in TLOD
        for (int z = 0; z < height; z++){
            for (int x = 0; x < width; x++){
                store.put( (float)x*scale.x );
                store.put( (float)0.0*scale.y );
                store.put( (float)z*scale.z );
            }
        }
        
        return store;
    }
    
    private static IntBuffer writeIndexArray(int width, int height){
        int faceN = (width-1)*(height-1)*2;
        
        IntBuffer store = BufferUtils.createIntBuffer(faceN*3);

        int i = 0;
        for (int z = 0; z < height-1; z++){
            for (int x = 0; x < width-1; x++){
                store.put(i).put(i+width).put(i+width+1);
                store.put(i+width+1).put(i+1).put(i);
                i++;
                
                if (x==width-2) i++;
            }
        }
        store.flip();
        
        return store;
    }
   
    private static FloatBuffer writeNormalArray(int width, int height) {
        FloatBuffer store = BufferUtils.createFloatBuffer(width*height*3);

        //Vector3f temp = new Vector3f();
        for (int z = 0; z < height; z++){
            for (int x = 0; x < width; x++){
//                getNormal(x,z,temp);
//                store.put(temp.x).put(temp.y).put(temp.z);
                store.put(0.0f).put(1.0f).put(0.0f);
            }
        }
        
        return store;
    }
    
    public static void rebuildNormalArray(FloatBuffer vb, FloatBuffer nb){
        vb.rewind();
        nb.rewind();
        
        Vector3f oppositePoint = new Vector3f();
        Vector3f adjacentPoint = new Vector3f();
        Vector3f rootPoint = new Vector3f();
        Vector3f tempNorm = new Vector3f();
        int adj = 0, opp = 0, normalIndex = 0;
        final int size = 64;
        
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                BufferUtils.populateFromBuffer(rootPoint, vb, normalIndex);
                if (row == size - 1) {
                    if (col == size - 1) { // last row, last col
                        // up cross left
                        adj = normalIndex - size;
                        opp = normalIndex - 1;
                    } else { // last row, except for last col
                        // right cross up
                        adj = normalIndex + 1;
                        opp = normalIndex - size;
                    }
                } else {
                    if (col == size - 1) { // last column except for last row
                        // left cross down
                        adj = normalIndex - 1;
                        opp = normalIndex + size;
                    } else { // most cases
                        // down cross right
                        adj = normalIndex + size;
                        opp = normalIndex + 1;
                    }
                }
                BufferUtils.populateFromBuffer(adjacentPoint, vb, adj);
                BufferUtils.populateFromBuffer(oppositePoint, vb, opp);
                tempNorm.set(adjacentPoint).subtractLocal(rootPoint)
                        .crossLocal(oppositePoint.subtractLocal(rootPoint))
                        .normalizeLocal();
                BufferUtils.setInBuffer(tempNorm, nb, normalIndex);
                normalIndex++;
            }
        }
    }
    
    private static FloatBuffer writeTexCoordArray(int width, int height, Vector2f offset, Vector2f scale){
        FloatBuffer store = BufferUtils.createFloatBuffer(width*height*2);
        
        if (offset == null)
            offset = new Vector2f();
        
        Vector2f tcStore = new Vector2f();
        for (int y = 0; y < height; y++){
            for (int x = 0; x < width; x++){
                tcStore.set( (float)x / (float)width,
                             (float)y / (float)height );
                store.put( offset.x + tcStore.x * scale.x );
                store.put( offset.y + tcStore.y * scale.y );
                
//                store.put( ((float)x) / getWidth() );
//                store.put( ((float)y) / getHeight() );
            }
            
        }
        
        return store;
    }
    
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
    
    public static Geometry calculatePick(Spatial scene, Ray r, Vector3f point){
        camResults.clear();
        scene.calculatePick(r, camResults);
        
        if (success){
            point.set(clickedLoc);
            return camResults.getPickData(0).getTargetMesh();
        }
        return null;
    }
    
    public static TriMesh createGrid(int width, int height, Vector3f scale, Vector2f[] tcScale) {
        TriMesh store = new TriMesh();

        store.setVertexBuffer(writeVertexArray(width,height,scale));
        store.setNormalBuffer(writeNormalArray(width,height));
        store.setIndexBuffer(writeIndexArray(width,height));
        
//        for (int i = 0; i < tcScale.length; i++){
//            TexCoords coords = new TexCoords(writeTexCoordArray(width,height,null,tcScale[i]),2);
//            store.setTextureCoords(coords, i);
//        }
        
        return store;
    }
    
}
