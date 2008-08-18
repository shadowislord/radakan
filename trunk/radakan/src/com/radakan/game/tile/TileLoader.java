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
import com.jme.math.Quaternion;
import com.jme.math.Vector2f;
import com.jme.math.Vector3f;
import com.jme.scene.Spatial;
import com.jme.scene.TexCoords;
import com.jme.scene.TriMesh;
import com.jme.util.resource.ResourceLocatorTool;
import com.jmex.terrain.TerrainBlock;
import com.radakan.graphics.mesh.parser.Material;
import com.radakan.graphics.mesh.parser.OgreLoader;
import com.radakan.util.ErrorHandler;
import com.radakan.util.XMLUtil;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.nio.FloatBuffer;
import java.util.Map;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class TileLoader {

    private Map<String, ItemType> itemMap;
    private Map<String, Material> materialMap;
    private boolean Z_up_to_Y_up = false;
    
    private int groupSize;
    private int tileSize;
    private int tileRes;
    
    private static final Quaternion conversion = new Quaternion();
    
    static {
        conversion.fromAngles(-FastMath.HALF_PI, FastMath.PI, 0);
    }
    
    public TileLoader(boolean Z_up){
        Z_up_to_Y_up = Z_up;
    }
    
    private Node getChildNode(Node node, String name) {
        Node child = node.getFirstChild();
        while (child != null && !child.getNodeName().equals(name) ){
            child = child.getNextSibling();
        }
        return child;
    }
    
    private String getAttribute(Node node, String name){
        Node att = node.getAttributes().getNamedItem(name);
        return att == null ? null : att.getNodeValue();
    }
    
    private TriMesh buildTerrain(int x, int y, float[] heights){
        int groupX = (int) Math.floor((float)x / groupSize);
        int groupY = (int) Math.floor((float)y / groupSize);
        
        int deltaX = x - groupX * groupSize;
        int deltaY = y - groupY * groupSize;
        
        Vector2f offset = new Vector2f((float)deltaX / groupSize,
                                       (float)deltaY / groupSize);
        Vector2f tcScale  = new Vector2f(1f / groupSize, 
                                         1f / groupSize);
        
        float tileScale = (float)tileSize / (tileRes-1.0f);
        
        TerrainBlock block = new TerrainBlock("terrain_"+x+"_"+y, 
                                              tileRes, 
                                              new Vector3f(-tileScale, 1f, -tileScale), 
                                              heights, 
                                              Vector3f.ZERO);
        
        FloatBuffer tc = GridUtil.writeTexCoordArray(tileRes, 
                                                     tileRes, 
                                                     tileRes-1,
                                                     tileRes-1,
                                                     offset, 
                                                     tcScale);
        
        TexCoords coords = new TexCoords(tc, 2);
        block.setTextureCoords(coords);
        
        return block;
    }
    
    private Spatial readTerrain(Node terrain, int x, int y){
       String[] usedMapsStr = XMLUtil.getAttribute(terrain, "usedmaps").split(",");
       String textureSet = XMLUtil.getAttribute(terrain, "textureset");
       int[] usedMaps = new int[usedMapsStr.length];
       
       for (int i = 0; i < usedMaps.length; i++) 
           usedMaps[i] = Integer.parseInt(usedMapsStr[i]);
       
       String heightsStr = terrain.getTextContent();
       
       float[] heights = new float[tileRes * tileRes];
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
       
       return buildTerrain(x, y, heights);
    }
    
    private Spatial readModel(Node model) {
        String item = getAttribute(model, "item");
        String modelName = getAttribute(model, "name");

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

        String[] pos = getAttribute(model, "position").split(",");
        String[] rot = getAttribute(model, "rotation").split(",");
        String[] scale = getAttribute(model, "scale").split(",");

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

    private Spatial readTile(Node tile, int x, int y){
        Node model = tile.getFirstChild();
        com.jme.scene.Node rootNode = new com.jme.scene.Node("tile_"+x+"_"+y);
        rootNode.setLocalTranslation(x * -tileSize, 0.0f, y * -tileSize);
        
        while (model != null){
            if (model.getNodeName().equals("model")){
                Spatial spat = readModel(model);
                rootNode.attachChild(spat);
            }else if (model.getNodeName().equals("terrain")){
                Spatial spat = readTerrain(model, x, y);
                rootNode.attachChild(spat);
            }
            
            model = model.getNextSibling();
        }
        
        return rootNode;
    }
    
    public void setTypes(Map<String, ItemType> typesMap){
        itemMap = typesMap;
    }
    
    public void setMaterials(Map<String, Material> materials){
        materialMap = materials;
    }
    
    private void readWorldMeta(Node world){
        groupSize = XMLUtil.getIntAttribute(world, "groupsize");
        tileSize = XMLUtil.getIntAttribute(world, "tilesize");
        tileRes = XMLUtil.getIntAttribute(world, "tileres");
    }
    
    public void readWorldMeta(URL url){
        InputStream in = null;
        try {
            in = url.openStream();

            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document doc = builder.parse(in);

            NodeList list = doc.getElementsByTagName("world");
            if (list.getLength() == 0) {
                throw new IOException("File does not contain metadata: " + url);
            }

            Node tile = list.item(0);
            readWorldMeta(tile);
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
    
    public Spatial loadTile(int x, int y){
        String name = "tile_"+x+"_"+y;
        // find the tile URL
        URL url = ResourceLocatorTool.locateResource("tile", name+".xml");
        
        if (url == null)
            throw new NullPointerException();
        
        try {
            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document doc = builder.parse(url.openStream());
            
            NodeList list = doc.getElementsByTagName("tile");
            if (list.getLength() == 0){
                return null;
            }
            
            Node tile = list.item(0);
            return readTile(tile, x, y);
        } catch (ParserConfigurationException ex) {
            ErrorHandler.reportError("XML reading error", ex);
        } catch (SAXException ex) {
            ErrorHandler.reportError("XML reading error", ex);
        } catch (IOException ex) {
            ErrorHandler.reportError("IO Error while reading tile", ex);
        }
        
        return null;
    }
    
}
