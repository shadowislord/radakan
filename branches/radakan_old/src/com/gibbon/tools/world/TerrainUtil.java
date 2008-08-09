package com.gibbon.tools.world;

import com.jme.bounding.CollisionTreeManager;
import com.jme.math.Vector2f;
import com.jme.math.Vector3f;
import com.jme.scene.Spatial;
import com.jme.scene.TexCoords;
import com.jme.scene.TriMesh;
import com.jme.util.geom.BufferUtils;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

public class TerrainUtil {

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
    
    public static void rebuildNormalArray(Tile tile){
        TriMesh mesh = tile.getTerrain();
        FloatBuffer vb = mesh.getVertexBuffer();
        FloatBuffer nb = mesh.getNormalBuffer();
        
        vb.rewind();
        nb.rewind();
        
        Vector3f oppositePoint = new Vector3f();
        Vector3f adjacentPoint = new Vector3f();
        Vector3f rootPoint = new Vector3f();
        Vector3f tempNorm = new Vector3f();
        int adj = 0, opp = 0, normalIndex = 0;
        final int size = World.getWorld().getGridResolution();
        
        int myX = tile.getX();
        int myY = tile.getY();
        
        Tile me = tile;
        Tile left =  World.getWorld().findTile(myX-1, myY);
        Tile right = World.getWorld().findTile(myX+1, myY);
        Tile down =  World.getWorld().findTile(myX, myY+1);
        Tile up =    World.getWorld().findTile(myX, myY-1);
        Tile sourceAdj = null;
        Tile sourceOpp = null;

        if (left != null && left.isModified()){
            rebuildNormalArray(left);
        }
        if (up != null && up.isModified()){
            rebuildNormalArray(up);
        }
        
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                BufferUtils.populateFromBuffer(rootPoint, vb, normalIndex);
                if (y == 0){
                    if (x == 0){
                        // get normal from top or left
                        if (up != null){
                            BufferUtils.populateFromBuffer(tempNorm, up.getTerrain().getNormalBuffer(), size*(size-1));
                            BufferUtils.setInBuffer(tempNorm, nb, normalIndex);
                            normalIndex++;
                            continue;
                        }else if (left != null){
                            BufferUtils.populateFromBuffer(tempNorm, left.getTerrain().getNormalBuffer(), size*(size-1)+size-1);
                            BufferUtils.setInBuffer(tempNorm, nb, normalIndex);
                            normalIndex++;
                            continue;
                        }
                    }else if (up != null){
                        BufferUtils.populateFromBuffer(tempNorm, up.getTerrain().getNormalBuffer(), size*(size-1)+x);
                        BufferUtils.setInBuffer(tempNorm, nb, normalIndex);
                        normalIndex++;
                        continue;
                    }
                }else if (x == 0){
                    if (left != null){
                        BufferUtils.populateFromBuffer(tempNorm, left.getTerrain().getNormalBuffer(), size*y+size-1);
                        BufferUtils.setInBuffer(tempNorm, nb, normalIndex);
                        normalIndex++;
                        continue;
                    }
                }
                
                if (y == size - 1) {
                    if (x == size - 1) { // last y, last x
                      //   up cross left
                      adj = normalIndex - size;
                      opp = normalIndex - 1;
                    } else { // last y, except for last x
                        // right cross up
                        adj = normalIndex + 1;
                        opp = normalIndex - size;
                    }
                } else {
                    if (x == size - 1) { // last x except for last y
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
    
    private static FloatBuffer writeTexCoordArray(int width, int height, int tWidth, int tHeight, Vector2f offset, Vector2f scale){
        FloatBuffer store = BufferUtils.createFloatBuffer(width*height*2);
        
        if (offset == null)
            offset = new Vector2f(0f, 0f);
        
        if (scale == null)
            scale = new Vector2f(1f, 1f);
        
        Vector2f tcStore = new Vector2f();
        for (int y = 0; y < height; y++){
            for (int x = 0; x < width; x++){
                tcStore.set( (float)x / (float)tWidth,
                             (float)y / (float)tHeight );
                store.put( offset.x + tcStore.x * scale.x );
                store.put( offset.y + tcStore.y * scale.y );
                
//                store.put( ((float)x) / getWidth() );
//                store.put( ((float)y) / getHeight() );
            }
            
        }
        
        return store;
    }
    
    public static TriMesh createGrid(int width, int height, Vector3f scale, Tile target) {
        int groupSize = World.getWorld().getGroupSize();
        
        int groupX = target.getGroup().getX();
        int groupY = target.getGroup().getY();
        
        int deltaX = target.getX() - groupX * groupSize;
        int deltaY = target.getY() - groupY * groupSize;
        
        Vector2f offset = new Vector2f((float)deltaX / groupSize,
                                       (float)deltaY / groupSize);
        Vector2f tcScale  = new Vector2f(1f / groupSize, 
                                         1f / groupSize);
        
        TriMesh store = new TriMesh("TERRAIN");

        store.setVertexBuffer(writeVertexArray(width,height,scale));
        store.setNormalBuffer(writeNormalArray(width,height));
        store.setIndexBuffer(writeIndexArray(width,height));
        
        FloatBuffer tc = writeTexCoordArray(width, 
                                            height, 
                                            width-1,
                                            height-1,
                                            offset, 
                                            tcScale);
        
        TexCoords coords = new TexCoords(tc, 2);
        store.setTextureCoords(coords);
        
//        for (int i = 0; i < tcScale.length; i++){
//            TexCoords coords = new TexCoords(writeTexCoordArray(width,height,null,tcScale[i]),2);
//            store.setTextureCoords(coords, i);
//        }
        
        return store;
    }
    
}
