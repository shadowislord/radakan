/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.gibbon.tools.world;

import com.jme.math.Vector2f;
import com.jme.math.Vector3f;
import com.jme.scene.TexCoords;
import com.jme.scene.TriMesh;
import com.jme.util.geom.BufferUtils;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

/**
 *
 * @author Kirill
 */
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
    
    public static void rebuildNormalArray(FloatBuffer vb, FloatBuffer nb){
        vb.rewind();
        nb.rewind();
        
        Vector3f oppositePoint = new Vector3f();
        Vector3f adjacentPoint = new Vector3f();
        Vector3f rootPoint = new Vector3f();
        Vector3f tempNorm = new Vector3f();
        int adj = 0, opp = 0, normalIndex = 0;
        final int size = World.getWorld().getGridResolution();
        
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
        
        int groupX = (int) Math.ceil((float)target.getX() / groupSize);
        int groupY = (int) Math.ceil((float)target.getY() / groupSize);
        
        int deltaX = (target.getX()-1) % groupSize;
        int deltaY = (target.getY()-1) % groupSize;
        
        Vector2f offset = new Vector2f((float)deltaX / groupSize,
                                       (float)deltaY / groupSize);
        Vector2f tcScale  = new Vector2f(1f / groupSize, 
                                         1f / groupSize);
        
        System.out.println(target.getName()+": "+offset);
        
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
