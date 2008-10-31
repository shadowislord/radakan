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

import com.jme.math.FastMath;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.logging.Logger;

import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.renderer.Renderer;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.state.CullState;
import com.jme.system.DisplaySystem;
import com.jme.util.resource.ResourceLocatorTool;
import com.radakan.game.util.IShadowManager;
import com.radakan.util.ErrorHandler;

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
    
    private static final Map<String, TextureSet> textureSets = new HashMap<String, TextureSet>();
    private static TileManager instance;
    
    private Renderer renderer;
    private float loadDistance;
    private float unloadDistance;
    private boolean enabled = false;
    
    private boolean useLightmaps;
    private boolean useFog;
    private int tileSize;
    private int groupSize;

    private IShadowManager shadowManager;
    
    /** 
     * Create a new tile manager 
     */
    public TileManager(Renderer renderer) {
        super("Tile Manager");
        if (instance != null)
            throw new IllegalStateException("Cannot create more than one TileManager");
        
        setRenderQueueMode(Renderer.QUEUE_OPAQUE);
        CullState cs = DisplaySystem.getDisplaySystem().getRenderer().createCullState();
        cs.setCullFace(CullState.Face.Back);
        setRenderState(cs);
        
        this.renderer = renderer;
        instance = this;
        logger.finest("TileManager created");
    }
    
    public static TileManager getInstance(){
        return instance;
    }
    
    public IShadowManager getShadowManager(){
        return shadowManager;
    }
    
    public void setShadowManager(IShadowManager manager){
        shadowManager = manager;
    }
    
    public TextureSet getTextureSet(String name){
        return textureSets.get(name);
    }
    
    public void loadDefaultTextureSets() throws IOException{
    	URL tsets = ResourceLocatorTool.locateResource("textureset", "texturesets.xml");
    	if (tsets == null)
    		throw new IOException("Cannot load texutresets; texturesets.xml is missing");
    	
    	InputStream in = tsets.openStream();
    	if (in == null)
    		throw new IOException("Cannot load texutresets; texturesets.xml is missing");
    	
        textureSets.putAll(TextureSetLoader.load(in, useLightmaps, useFog));
    	
    	in.close();
    }
    
    public void setEnabled(boolean enable){
        enabled = enable;
    }
    
    public boolean isEnabled(){
        return enabled;
    }
    
    public void setUseLightmaps(boolean use){
        useLightmaps = use;
    }
    
    public void setUseFog(boolean use){
        useFog = use;
    }
    
    public void setTileSize(int tileSize){
        if (tileSize < 5)
            throw new IllegalArgumentException("Tile size is too low: "+tileSize);

        this.tileSize = tileSize;
    }
    
    public void setGroupSize(int groupSize){
        if (groupSize < 1)
            throw new IllegalArgumentException("Group size is too low: "+groupSize);
            
        this.groupSize = groupSize;
    }
    
    public float getLoadDistance(){
        return loadDistance;
    }
    
    public float getUnloadDistance(){
        return unloadDistance;
    }
    
    public void setLoadDistance(float loadDistance){
        this.loadDistance = loadDistance;
    }
    
    public void setUnloadDistance(float unloadDistance){
        this.unloadDistance = unloadDistance;
    }
    
    public boolean usingLightmaps(){
        return useLightmaps;
    }
    
    public int getTileSize(){
        return tileSize;
    }
    
    public int getGroupSize(){
        return groupSize;
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
        return new Vector3f(x * -tileSize, 0f, y * -tileSize);
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
        
//        newTile.lockBounds();
//        newTile.lockTransforms();
//        newTile.lockBranch();
        
        return true;
    }

    public float getTerrainHeight(Vector3f point){
        int x = (int) FastMath.floor(point.x / -tileSize);
        int y = (int) FastMath.floor(point.z / -tileSize);
        Tile t = getTileAt(x, y);
        if (t != null)
            return t.getTerrainHeight(point);
        else{
            return Float.NaN;
        }
    }
    
    @Override
    public void updateGeometricState(float tpf, boolean initiator) {
        super.updateGeometricState(tpf, initiator);
        
        if (!enabled)
            return;
        
        Camera cam = renderer.getCamera();
        
        Vector3f temp = new Vector3f(),
                 temp2 = new Vector3f();
        
        if (children != null) {
            // must keep a copy of the child list since
            // it is going to be modified while being iterated
            Spatial[] localArray = new Spatial[children.size()];
            children.toArray(localArray);
            
            for (Spatial child : localArray) {
                Tile tile = (Tile) child;
                temp.set(tile.getLocalTranslation());
                // find center of tile
                temp.subtractLocal(tileSize * .5f, 0f, tileSize * .5f);
                
                // set y to zero, only want to check distance on XY plane
                temp.y = 0f;
                temp2.set(cam.getLocation()).y = 0f;
                if (temp2.distance(temp) > unloadDistance){
                    unloadAndDetach(tile);
                }
            }
        }
        
        Vector3f loadStart = new Vector3f(cam.getLocation()).subtractLocal(loadDistance, 0f, loadDistance);
        Vector3f loadEnd = new Vector3f(cam.getLocation()).addLocal(loadDistance, 0f, loadDistance);
        
        int loadStartTileX = Math.round(loadStart.x / -tileSize);
        int loadStartTileY = Math.round(loadStart.z / -tileSize);
        int loadEndTileX = Math.round(loadEnd.x / -tileSize);
        int loadEndTileY = Math.round(loadEnd.z / -tileSize);

        for (int y = loadEndTileY; y <= loadStartTileY; y++){
            for (int x = loadEndTileX; x <= loadStartTileX; x++){
                temp.set(tileToWorld(x, y));
                temp.subtractLocal(tileSize * .5f, 0f, tileSize * .5f);
                
                temp.y = 0f;
                temp2.set(cam.getLocation()).y = 0f;
                
                if (temp2.distance(temp) <= loadDistance){
                    loadAndAttach(x,y);
                }
            }
        }
        
//        for (Spatial child : children){
//            Tile t = (Tile) child;
//            t.updateWorldData(tpf);
//        }
        
        if (shadowManager != null)
            shadowManager.update(tpf);
    }
    
}
