/*
 * Radakan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.radakan.game.tile;

import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.renderer.Renderer;
import com.jme.scene.Node;
import com.jme.scene.Spatial;

import java.util.logging.Logger;
import static java.lang.Math.*;

/**In Middle of creation
 * 
 * Changes by Kirill (MomokoFan)
 * - Now extends Node
 * - Some docs
 * 
 * @author Kirill (MomokoFan)
 * @created Aug 18, 2008
 */
public class TileManager extends Node {

    private static final Logger logger = Logger.getLogger(TileManager.class.getName());
    
    private static TileManager instance;
    
    private Renderer renderer;
    private float loadDistance = 64f * 1f;

    /** 
     * Create a new tile manager 
     */
    public TileManager(Renderer renderer) {
        if (instance != null)
            throw new IllegalStateException("Cannot create more than one TileManager");
        
        this.renderer = renderer;
        instance = this;
        logger.finest("TileManager created");
    }
    
    /**
     * Returns the tile at position X, Y or null if no tile is available at the position
     */
    public Tile getTileAt(int x, int y) {
        return (Tile) getChild("TILE_" + x + "_" + y);
    }
    
    /**
     * Converts a location in tile space to world space.
     */
    public Vector3f tileToWorld(int x, int y){
        return new Vector3f(x * -Tile.TILE_SIZE, 0f, y * -Tile.TILE_SIZE);
    }
    
    /**
     * Deletes a tile from the tile manager
     */
    protected boolean unloadAndDetach(Tile tile){
        detachChild(tile);
        return tile.unload();
    }
    
    /**
     * Loads and attaches the tile at the specified position
     */
    protected boolean loadAndAttach(int x, int y) {
        if (getTileAt(x,y) != null)
            return false;
        
        Tile newTile = new Tile(x,y);
        if (!newTile.load())
            return false;
        
        // find tile world position
        newTile.setLocalTranslation(tileToWorld(x,y));
        
        // attach new tile
        attachChild(newTile);

        newTile.updateRenderState();
        newTile.updateGeometricState(0, true);
        
        newTile.lockBounds();
        newTile.lockTransforms();
        newTile.lockBranch();
        
        return true;
    }

    @Override
    public void updateWorldData(float tpf) {
        Camera cam = renderer.getCamera();
        
        Vector3f temp = new Vector3f(),
                 temp2 = new Vector3f();
        
//        if (children != null) {
//            // must keep a copy of the child list since
//            // it is going to be modified while being iterated
//            Spatial[] localArray = new Spatial[children.size()];
//            children.toArray(localArray);
//            
//            for (Spatial child : localArray) {
//                Tile tile = (Tile) child;
//                temp.set(tile.getLocalTranslation());
//                // find center of tile
//                temp.subtractLocal(Tile.TILE_SIZE * .5f, 0f, Tile.TILE_SIZE * .5f);
//                
//                // set y to zero, only want to check distance on XY plane
//                temp2.set(cam.getLocation()).y = 0f;
//                if (temp2.distance(temp) > loadDistance){
//                    unloadAndDetach(tile);
//                }
//            }
//        }
        
        Vector3f loadStart = new Vector3f(cam.getLocation()).subtractLocal(loadDistance, 0f, loadDistance);
        Vector3f loadEnd = new Vector3f(cam.getLocation()).addLocal(loadDistance, 0f, loadDistance);
        
        int loadStartTileX = Math.round(loadStart.x / -Tile.TILE_SIZE);
        int loadStartTileY = Math.round(loadStart.z / -Tile.TILE_SIZE);
        int loadEndTileX = Math.round(loadEnd.x / -Tile.TILE_SIZE);
        int loadEndTileY = Math.round(loadEnd.z / -Tile.TILE_SIZE);
        
        //System.out.println(loadStartTileX + ", " + loadStartTileY + "     " + loadEndTileX + ", "+loadEndTileY);
        
        for (int y = loadEndTileY; y <= loadStartTileY; y++){
            for (int x = loadEndTileX; x <= loadStartTileX; x++){
                 loadAndAttach(x,y);
            }
        }
    }
    
}