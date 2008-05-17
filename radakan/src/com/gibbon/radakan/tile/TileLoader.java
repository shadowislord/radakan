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

package com.gibbon.radakan.tile;

import com.gibbon.meshparser.Material;
import com.gibbon.meshparser.OgreLoader;
import com.gibbon.radakan.error.ErrorReporter;
import com.gibbon.radakan.entity.*;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.scene.Spatial;
import com.jme.util.resource.ResourceLocatorTool;
import java.io.IOException;
import java.net.URL;
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
    
    private Spatial readTile(Node tile, String name){
        Node model = tile.getFirstChild();
        com.jme.scene.Node rootNode = new com.jme.scene.Node(name);
        
        while (model != null){
            if (model.getNodeName().equals("model")){
                String item = getAttribute(model, "item");
                String modelName = getAttribute(model, "name");
                
                ItemType type = itemMap.get(item);
                
                if (type == null){
                    throw new IllegalStateException("Cannot find type: "+item);
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
                
                if (Z_up_to_Y_up){
                    spatial.setLocalTranslation(-Float.parseFloat(pos[0]),
                                                Float.parseFloat(pos[2]),
                                                Float.parseFloat(pos[1]));
                    
                }else{
                  spatial.setLocalTranslation(Float.parseFloat(pos[0]),
                                              Float.parseFloat(pos[1]),
                                              Float.parseFloat(pos[2]));
                }
                
//                  conversion.multLocal(spatial.getLocalTranslation());
                  
//                if (Z_up_to_Y_up){
//                    spatial.getLocalRotation().fromAngles(Float.parseFloat(rot[0]),
//                                                          Float.parseFloat(rot[2]),
//                                                          -Float.parseFloat(rot[1]));
//                    
////                    spatial.getLocalRotation().set(Float.parseFloat(rot[0]),
////                                                   Float.parseFloat(rot[2]),
////                                                   Float.parseFloat(rot[1]),
////                                                   Float.parseFloat(rot[3]));
//                }else{
//                    spatial.getLocalRotation().fromAngles(Float.parseFloat(rot[0]),
//                                                          Float.parseFloat(rot[1]),
//                                                          Float.parseFloat(rot[2]));
                    spatial.getLocalRotation().set(Float.parseFloat(rot[0]),
                                                   Float.parseFloat(rot[1]),
                                                   Float.parseFloat(rot[2]),
                                                   Float.parseFloat(rot[3]));
//                }
                
//                    spatial.getLocalRotation().multLocal(conversion);
                spatial.getLocalRotation().normalize();
                
                if (Z_up_to_Y_up){
                    spatial.getLocalScale().set(Float.parseFloat(scale[0]),
                                                Float.parseFloat(scale[2]),
                                                Float.parseFloat(scale[1]));
                }else{
                    spatial.getLocalScale().set(Float.parseFloat(scale[0]),
                                                Float.parseFloat(scale[1]),
                                                Float.parseFloat(scale[2]));
                }
                
//                System.out.println("--"+spatial.getName()+"--");
//                System.out.println("Position: "+spatial.getLocalTranslation());
//                System.out.println("Scale: "+spatial.getLocalRotation());
                    
                rootNode.attachChild(spatial);
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
            return readTile(tile, name);
        } catch (ParserConfigurationException ex) {
            ErrorReporter.reportError("XML reading error", ex);
        } catch (SAXException ex) {
            ErrorReporter.reportError("XML reading error", ex);
        } catch (IOException ex) {
            ErrorReporter.reportError("IO Error while reading tile", ex);
        }
        
        return null;
    }
    
}
