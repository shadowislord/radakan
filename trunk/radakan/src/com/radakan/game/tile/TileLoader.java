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

import com.jme.image.Image;
import com.jme.image.Texture2D;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector2f;
import com.jme.math.Vector3f;
import com.jme.renderer.lwjgl.LWJGLRenderer;
import com.jme.scene.Spatial;
import com.jme.scene.TexCoords;
import com.jme.scene.TriMesh;
import com.jme.scene.VBOInfo;
import com.jme.scene.state.TextureState;
import com.jme.system.DisplaySystem;
import com.jme.util.TextureManager;
import com.jme.util.resource.ResourceLocatorTool;
import com.jmex.terrain.TerrainBlock;
import com.radakan.entity.EntityFactory;
import com.radakan.graphics.mesh.parser.Material;
import com.radakan.graphics.mesh.parser.OgreLoader;
import com.radakan.util.ErrorHandler;
import com.radakan.util.XMLUtil;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.nio.FloatBuffer;
import java.util.Map;
import java.util.logging.Logger;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class TileLoader {
    
    private static final Logger logger = Logger.getLogger(TileLoader.class.getName());

    private Map<String, ItemType> itemMap;
    private Map<String, Material> materialMap;
    
    public TileLoader(){
    }
    
    private TriMesh buildTerrain(int x, int y, float[] heights){
        int groupX = (int) Math.floor((float)x / Tile.GROUP_SIZE);
        int groupY = (int) Math.floor((float)y / Tile.GROUP_SIZE);
        
        int deltaX = x - groupX * Tile.GROUP_SIZE;
        int deltaY = y - groupY * Tile.GROUP_SIZE;
        
        Vector2f offset = new Vector2f((float)deltaX / Tile.GROUP_SIZE,
                                       (float)deltaY / Tile.GROUP_SIZE);
        Vector2f tcScale  = new Vector2f(1f / Tile.GROUP_SIZE, 
                                         1f / Tile.GROUP_SIZE);
        
        float tileScale = (float)Tile.TILE_SIZE / (Tile.TILE_RESOLUTION-1.0f);
        
        TerrainBlock block = new TerrainBlock("terrain_"+x+"_"+y, 
                                              Tile.TILE_RESOLUTION, 
                                              new Vector3f(-tileScale, 1f, -tileScale), 
                                              heights, 
                                              Vector3f.ZERO);
        
        FloatBuffer tc = GridUtil.writeTexCoordArray(Tile.TILE_RESOLUTION, 
                                                     Tile.TILE_RESOLUTION, 
                                                     Tile.TILE_RESOLUTION-1,
                                                     Tile.TILE_RESOLUTION-1,
                                                     offset, 
                                                     tcScale);
        
        TexCoords coords = new TexCoords(tc, 2);
        block.setTextureCoords(coords);
        
        return block;
    }
    
    private Spatial readTerrain(Node terrain, int x, int y){
        // load texture set and related data
       String[] usedMapsStr = XMLUtil.getAttribute(terrain, "usedmaps").split(",");
       String textureSet = XMLUtil.getAttribute(terrain, "textureset");
       int[] usedMaps = new int[usedMapsStr.length];
       for (int i = 0; i < usedMaps.length; i++) 
           usedMaps[i] = Integer.parseInt(usedMapsStr[i].trim());
       
        TextureSet set = Tile.TEXTURE_SETS.get(textureSet);
        TextureState state = DisplaySystem.getDisplaySystem().getRenderer().createTextureState();
        if (set == null){
            logger.warning("Failed to locate textureset "+textureSet);
        }else{
            int groupX = (int) Math.floor((float)x / Tile.GROUP_SIZE);
            int groupY = (int) Math.floor((float)y / Tile.GROUP_SIZE);
            
            Texture2D[] alphamaps = set.createStateCopy(state);

            // only load the alphamaps for the used textures
            for (int i = 0; i < usedMaps.length; i++){
                // example name: GROUP_1_1_3.png
                String mapName = "alpha_" + groupX + "_" + groupY + "_" + usedMaps[i] + ".png";

                // load the alphamap, and assign it to the correct slot in the textureset
                Image alphaimage = TextureManager.loadImage(mapName, false);
                alphamaps[usedMaps[i]].setImage(alphaimage);
            }
            
            // replace lightmap too
            if (Tile.USE_LIGHTMAP){
                String mapName = "light_"+ groupX + "_" + groupY + ".png";

                Image lightimage = TextureManager.loadImage(mapName, false);

                if (lightimage != null){
                    // last alphamap is actually a lightmap
                    alphamaps[alphamaps.length-1].setImage(lightimage);
                }
            }
        }
        
        
        // XXX: might want to add a state.load() call here to upload
        // all alphamaps to the GPU
        
       // load heightmap
       String heightsStr = terrain.getTextContent();
       
       float[] heights = new float[Tile.TILE_RESOLUTION * Tile.TILE_RESOLUTION];
       int start = 0;
       for (int i = 0; i < heights.length; i++){
           String buf = "";
           for (int j = start; j < heightsStr.length(); j++){
               if (heightsStr.charAt(j) == ' '){
                   start = j + 1;
                   break;
               }else{
                   buf += heightsStr.charAt(j);
               }
           }
           heights[i] = Float.parseFloat(buf);
       }
       
       TriMesh terrainMesh = buildTerrain(x, y, heights);
       
       if (set != null){
           // apply the texture set
           terrainMesh.setRenderState(state);
           terrainMesh.setRenderState(set.getShader());
       }
       
       // make sure to clear that..
       VBOInfo info = new VBOInfo(true);
       info.setVBOIndexEnabled(true);
       terrainMesh.setVBOInfo(info);
       
       return terrainMesh;
    }
    
    @Deprecated
    private Spatial readModel(Node model) {
        String item = XMLUtil.getAttribute(model, "item");
        String modelName = XMLUtil.getAttribute(model, "name");

        ItemType type = itemMap.get(item);

        if (type == null) {
            throw new IllegalStateException("Cannot find type: " + item);
        }

        URL url = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, type.meshName + ".xml");
        OgreLoader loader = new OgreLoader();
        loader.setMaterials(materialMap);
        Spatial spatial = loader.loadModel(url, false);
        spatial.setName(modelName);
        spatial.setIsCollidable(type.solid);

        String[] pos = XMLUtil.getAttribute(model, "position").split(",");
        String[] rot = XMLUtil.getAttribute(model, "rotation").split(",");
        String[] scale = XMLUtil.getAttribute(model, "scale").split(",");

        spatial.setLocalTranslation(Float.parseFloat(pos[0]),
                Float.parseFloat(pos[1]),
                Float.parseFloat(pos[2]));


        spatial.getLocalRotation().set(Float.parseFloat(rot[0]),
                Float.parseFloat(rot[1]),
                Float.parseFloat(rot[2]),
                Float.parseFloat(rot[3]));

        spatial.getLocalScale().set(Float.parseFloat(scale[0]),
                Float.parseFloat(scale[1]),
                Float.parseFloat(scale[2]));

        return spatial;
    }

    private void readTile(Node tile, int x, int y, Tile target){
        Node model = tile.getFirstChild();
        while (model != null){
            //if (model.getNodeName().equals("model")){
            //    Spatial spat = readModel(model);
            //    target.addObject(spat);
            //}else 
            if (model.getNodeName().equals("terrain")){
                Spatial spat = readTerrain(model, x, y);
                target.setTerrain(spat);
            }else if (model.getNodeName().equals("entity")){
                
            }
            
            model = model.getNextSibling();
        }
    }
    
    public void setTypes(Map<String, ItemType> typesMap){
        itemMap = typesMap;
    }
    
    public void setMaterials(Map<String, Material> materials){
        materialMap = materials;
    }
    
    public static void readEntityTypes(File rootFolder){
        try{
            for (File f : rootFolder.listFiles()){
                if (f.getName().toLowerCase().endsWith(".xml")){
                    InputStream in = new FileInputStream(f);
                    EntityFactory.getInstance().load(in);
                    in.close();
                }
            }
        } catch (IOException ex){
            ErrorHandler.reportError("Error while reading TextureSets", ex);
        }
    }
    
    public static void readTextureSets(File rootFolder){
        try{
            for (File f : rootFolder.listFiles()){
                if (f.getName().toLowerCase().endsWith(".xml")){
                    InputStream in = new FileInputStream(f);
                    TextureSet set = TextureSetLoader.load(in, Tile.USE_LIGHTMAP);
                    Tile.TEXTURE_SETS.put(set.toString(), set);
                    in.close();
                }
            }
        } catch (IOException ex){
            ErrorHandler.reportError("Error while reading TextureSets", ex);
        }
    }
    
    public static void readWorldMeta(URL url){
        InputStream in = null;
        try {
            in = url.openStream();

            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document doc = builder.parse(in);

            NodeList list = doc.getElementsByTagName("world");
            if (list.getLength() == 0) {
                throw new IOException("File does not contain metadata: " + url);
            }

            Node worldNode = list.item(0);
            Tile.GROUP_SIZE = XMLUtil.getIntAttribute(worldNode, "groupsize");
            Tile.TILE_SIZE = XMLUtil.getIntAttribute(worldNode, "tilesize");
            Tile.TILE_RESOLUTION = XMLUtil.getIntAttribute(worldNode, "tileres");
            if (!XMLUtil.getAttribute(worldNode, "haslightmaps").equals("false")){
                Tile.USE_LIGHTMAP = true;
            }
        } catch (SAXException ex) {
            ErrorHandler.reportError("Error while parsing XML file", ex);
        } catch (IOException ex) {
            ErrorHandler.reportError("Error while parsing XML file", ex);
        } catch (ParserConfigurationException ex) {
            ErrorHandler.reportError("Error while parsing XML file", ex);
        } finally {
            try {
                in.close();
            } catch (IOException ex) {
                ErrorHandler.reportError("Error while parsing XML file", ex);
            }
        }
    }
    
    public boolean loadTile(int x, int y, InputStream in, Tile target){
        try {
            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document doc = builder.parse(in);
            
            NodeList list = doc.getElementsByTagName("tile");
            if (list.getLength() == 0){
                return false;
            }
            
            Node tileNode = list.item(0);
            readTile(tileNode, x, y, target);
            
            return true;
        } catch (ParserConfigurationException ex) {
            ErrorHandler.reportError("XML reading error", ex);
        } catch (SAXException ex) {
            ErrorHandler.reportError("XML reading error", ex);
        } catch (IOException ex) {
            ErrorHandler.reportError("IO Error while reading tile", ex);
        }
        
        return false;
    }
    
}
