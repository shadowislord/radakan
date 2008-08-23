package com.gibbon.tools.world;

import com.jme.bounding.CollisionTreeManager;
import com.jme.math.Vector2f;
import com.jme.math.Vector3f;
import com.jme.scene.Spatial;
import com.jme.scene.TexCoords;
import com.jme.scene.TriMesh;
import com.jme.util.geom.BufferUtils;
import com.radakan.game.tile.GridUtil;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

public class TerrainUtil {

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
                float h = PickUtils.getTerrainHeight(model.getWorldTranslation(), null, PickUtils.CRASH_IF_NAN);
                model.getLocalTranslation().y = h;
            }
        }
        
        tile.clearModified();
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

        store.setVertexBuffer(GridUtil.writeVertexArray(width,height,scale));
        store.setNormalBuffer(GridUtil.writeNormalArray(width,height));
        store.setIndexBuffer(GridUtil.writeIndexArray(width,height));
        
        FloatBuffer tc = GridUtil.writeTexCoordArray(width, 
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
