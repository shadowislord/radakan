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

import com.jme.bounding.BoundingBox;
import com.jme.image.Image;
import com.jme.image.Texture2D;
import com.jme.math.Vector2f;
import com.jme.math.Vector3f;
import com.jme.renderer.Renderer;
import com.jme.scene.Spatial;
import com.jme.scene.TexCoords;
import com.jme.scene.TriMesh;
import com.jme.scene.VBOInfo;
import com.jme.scene.state.RenderState;
import com.jme.scene.state.TextureState;
import com.jme.scene.state.lwjgl.LWJGLTextureState;
import com.jme.system.DisplaySystem;
import com.jme.util.TextureManager;
import com.jme.util.resource.ResourceLocatorTool;
import com.jmex.terrain.TerrainBlock;
import com.radakan.entity.Entity;
import com.radakan.entity.EntityManager;
import com.radakan.entity.unit.ModelUnit;
import com.radakan.game.util.IShadowManager;
import com.radakan.graphics.util.ModelCloneUtil;
import com.radakan.util.ErrorHandler;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.nio.FloatBuffer;
import java.util.logging.Logger;

import org.w3c.dom.Node;
import static com.radakan.util.XMLUtil.*;

/**
 * @author Joshua Montgomery
 * @created Aug 18, 2008
 */
public class Tile extends com.jme.scene.Node{
    
	private static final long serialVersionUID = 5468917005537121768L;

        private static final Logger logger = Logger.getLogger(Tile.class.getName());
        
        /**
         * Terrain model
         */
        private TriMesh terrain;
        
        /**
         * The texture set which is used to splat the terrain.
         */
        private TextureSet textureSet;
        
        /**
         * The tile location in tile space.
         */
        public final int x, y;
        
        public Tile(int x, int y){
            super("TILE_"+x+"_"+y);
            this.x = x;
            this.y = y;
        }
        
        /**
         * Add a new object (entity or static) onto the terrain.
         * @param object The object to add
         */
        public void addObject(Spatial object){
            attachChild(object);
            IShadowManager manager = TileManager.getInstance().getShadowManager();
            if (manager != null){
                manager.addOccluder(object);
                manager.addShadowReciever(object);
            }
        }
        
        /**
         * Sets the terrain model of this Tile.
         * @param terrain The terrain model to set
         */
        public void setTerrain(TriMesh terrain){
            this.terrain = terrain;
            attachChildAt(terrain, 0);
            IShadowManager manager = TileManager.getInstance().getShadowManager();
            if (manager != null){
                TriMesh clonedTerrain = (TriMesh) ModelCloneUtil.cloneSmart(terrain);
                clonedTerrain.clearRenderState(RenderState.RS_TEXTURE);
                clonedTerrain.clearRenderState(RenderState.RS_GLSL_SHADER_OBJECTS);
                clonedTerrain.updateGeometricState(0, true);
                manager.addShadowReciever(clonedTerrain);
            }
        }
        
    private TriMesh loadTerrainBlock(float[] heights, int tileRes) {
        int groupSize = TileManager.getInstance().getGroupSize();
        int tileSize = TileManager.getInstance().getTileSize();

        int groupX = (int) Math.floor((float) x / groupSize);
        int groupY = (int) Math.floor((float) y / groupSize);

        int deltaX = x - groupX * groupSize;
        int deltaY = y - groupY * groupSize;

        Vector2f offset = new Vector2f((float) deltaX / groupSize,
                (float) deltaY / groupSize);
        Vector2f tcScale = new Vector2f(1f / groupSize,
                1f / groupSize);

        float tileScale = (float) tileSize / (tileRes - 1.0f);

        TerrainBlock block = new TerrainBlock("terrain_" + x + "_" + y,
                                              tileRes,
                                              new Vector3f(-tileScale, 1f, -tileScale),
                                              heights,
                                              Vector3f.ZERO);

        FloatBuffer tc = GridUtil.writeTexCoordArray(tileRes,
                                                     tileRes,
                                                     tileRes - 1,
                                                     tileRes - 1,
                                                     offset,
                                                     tcScale);

        TexCoords coords = new TexCoords(tc, 2);
        block.setTextureCoords(coords);

        return block;
    }
    
    private TriMesh loadTerrainBlock(Node terrainXMLNode){
        // load heightmap
        int size = getIntAttribute(terrainXMLNode, "size");
        String heightsStr = terrainXMLNode.getTextContent();

        float[] heights = new float[size * size];
        int start = 0;
        for (int i = 0; i < heights.length; i++) {
            String buf = "";
            for (int j = start; j < heightsStr.length(); j++) {
                if (heightsStr.charAt(j) == ' ') {
                    start = j + 1;
                    break;
                } else {
                    buf += heightsStr.charAt(j);
                }
            }
            heights[i] = Float.parseFloat(buf);
        }

        return loadTerrainBlock(heights, size);
    }
    
    private void loadTexturing(Node terrainXMLNode){
        // load texture set and related data
        String[] usedMapsStr = getAttribute(terrainXMLNode, "usedmaps", "").split(",");
        int[] usedMaps = null;
        if (usedMapsStr.length > 0){
            usedMaps = new int[usedMapsStr.length];
            for (int i = 0; i < usedMaps.length; i++) {
                usedMaps[i] = Integer.parseInt(usedMapsStr[i].trim());
            }
        }

        String textureSetName = getAttribute(terrainXMLNode, "textureset");
        textureSet = TileManager.getInstance().getTextureSet(textureSetName);
        
        if (textureSet == null) {
            logger.warning("Failed to locate textureset " + textureSetName);
        } else {
            TextureState state = DisplaySystem.getDisplaySystem().getRenderer().createTextureState();
            
            int groupSize = TileManager.getInstance().getGroupSize();
            int groupX = (int) Math.floor((float) x / groupSize);
            int groupY = (int) Math.floor((float) y / groupSize);

            Texture2D[] alphamaps = textureSet.createStateCopy(state);

            if (usedMaps != null){
                // only load the alphamaps for the used textures
                for (int i = 0; i < usedMaps.length; i++) {
                    // example name: GROUP_1_1_3.png
                    String mapName = "alpha_" + groupX + "_" + groupY + "_" + usedMaps[i] + ".png";

                    // load the alphamap, and assign it to the correct slot in the textureset
                    Image alphaimage = TextureManager.loadImage(mapName, false);
                    alphamaps[usedMaps[i]].setImage(alphaimage);
                }
            }
            
            // replace lightmap if used
            if (TileManager.getInstance().usingLightmaps()) {
                String mapName = "light_" + groupX + "_" + groupY + ".png";

                Image lightimage = TextureManager.loadImage(mapName, false);

                if (lightimage != null) {
                    // last alphamap is actually a lightmap
                    alphamaps[alphamaps.length - 1].setImage(lightimage);
                }
            }
            
            // apply texturing to terrain
            if (textureSet != null) {
                terrain.setRenderState(state);
                terrain.setRenderState(textureSet.getShader());
            }
        }
    }
    
    private void loadTerrainWithTexturing(Node terrainXMLNode) {
        setTerrain(loadTerrainBlock(terrainXMLNode));
        loadTexturing(terrainXMLNode);
        
        VBOInfo vbo = new VBOInfo(true);
        vbo.setVBOIndexEnabled(true);
        terrain.setVBOInfo(vbo);
    }
    
    private void loadEntity(Node entityXMLNode){
        Entity ent = EntityManager.realize(entityXMLNode);
        ModelUnit model = ent.getModel();
        ent.birth();
        addObject(model.getModel());
    }

    protected void loadTile(Node tileXMLNode) {
        Node modelOrTerrainXMLNode = tileXMLNode.getFirstChild();
        while (modelOrTerrainXMLNode != null) {
            if (modelOrTerrainXMLNode.getNodeName().equals("entity")){
                loadEntity(modelOrTerrainXMLNode);
            }else if (modelOrTerrainXMLNode.getNodeName().equals("terrain")) {
                loadTerrainWithTexturing(modelOrTerrainXMLNode);
            }

            modelOrTerrainXMLNode = modelOrTerrainXMLNode.getNextSibling();
        }
    }
    
    public void genBounds(){
        setModelBound(new BoundingBox());
        updateModelBound();
    }
    
    public URL locateTile() throws IOException{
        return ResourceLocatorTool.locateResource("tile", getName() + ".xml");
    }

    /**
     * Loads the tile, making it contain valid renderable information.
     * This operation is atomic; at the end of the execution of this method,
     * the tile scene graph will contain all valid geometry and render states.
     * 
     * @return True if the tile has been loaded successfuly and the data is valid.
     */
    public boolean load() {
        try {
            URL url = locateTile();
            if (url == null)
                return false;
            
            InputStream in = url.openStream();
            if (in == null)
                return false;
            
            loadTile(loadDocument(in, "tile"));
            in.close();
            
            logger.finest(getName()+" loaded successfuly.");

            return true;
        } catch (IOException ex) {
            ErrorHandler.reportError("Error while reading " + getName(), ex);
        }

        return false;
    }

        /**
         * Unloads the tile, this method can implement (if desired)
         * some sort of caching so that the tile is not unloaded immediately. 
         * However it is still expected that the tile scene graph will not be
         * renderable at the end of the execution of this method.
         * 
         * @return True if the tile has been unloaded successfuly. False if an error occured.
         */
        public boolean unload(){
            logger.finest(getName() + " detached.");
            
            Renderer r = DisplaySystem.getDisplaySystem().getRenderer();
            if (r.supportsVBO()){
                // make sure to unload VBO if set
                VBOInfo vbo = terrain.getVBOInfo();
                if (vbo != null){
                    r.deleteVBO(vbo.getVBOVertexID());
                    r.deleteVBO(vbo.getVBOColorID());
                    r.deleteVBO(vbo.getVBOFogCoordsID());
                    r.deleteVBO(vbo.getVBOIndexID());
                    r.deleteVBO(vbo.getVBONormalID());
                    r.deleteVBO(vbo.getVBOTextureID(0));
                }
            }
            
            return true;
        }
        
}
