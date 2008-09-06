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

package com.radakan.entity;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.Callable;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.xml.sax.SAXException;


import com.gibbon.jme.context.JmeContext;
import com.jme.bounding.BoundingBox;
import com.jme.scene.Spatial;
import com.jme.util.resource.ResourceLocatorTool;
import com.radakan.entity.unit.ModelUnit;
import com.radakan.graphics.util.ModelCloneUtil;
import com.radakan.graphics.util.ModelLoader;
import com.radakan.util.ErrorHandler;
import com.radakan.util.XMLUtil;
import java.net.URL;

public final class EntityManager {

    private static final Map<String, EntityType> entityTypeMap = new HashMap<String, EntityType>();
    
    public static class EntityType {
        // entity params
        public String name;
        public String type;
        public String category;
        
        // model params
        public com.jme.scene.Node model = null;
        public String modelName;
        public float scaleStart = 1.0f, 
                     scaleEnd   = 1.0f;
        public float height = 0.0f;
        public float normal_match = 1.0f;
        
        @Override
        public String toString(){
            return name;
        }
    }

    private static Spatial loadModel(String path) throws IOException {
        if (path.startsWith("/models/"))
            path = path.substring(8);
        
        URL url = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, path);
        if (url == null)
            throw new IOException("Cannot locate model "+path);
        
        // path is local to the file system
        int index = path.indexOf(".");
        if (index == -1)
            throw new IOException("No extension available for model file");
        
        String ext = path.substring(index+1);
        return ModelLoader.loadModel(url, ext);
    }
    
    private static void loadEntityTypes(Node entitiesNode) throws IOException{
        Node entityNode = entitiesNode.getFirstChild();
        while (entityNode != null){
            if (entityNode.getNodeName().equals("entitytype")){
                EntityType ent = new EntityType();
                
                ent.name = XMLUtil.getAttribute(entityNode, "name");
                ent.type = XMLUtil.getAttribute(entityNode, "type");
                ent.category = XMLUtil.getAttribute(entityNode, "category");
                
                Node modelNode = XMLUtil.getChildNode(entityNode, "model");
                if (modelNode != null){
                    float scale  = XMLUtil.getFloatAttribute(modelNode, "scale", 1.0f);
                    float height = XMLUtil.getFloatAttribute(modelNode, "height", 0.0f);
                    
                    ent.modelName = XMLUtil.getAttribute(modelNode, "src");
                    final Spatial model = loadModel(ent.modelName);
                    com.jme.scene.Node node = null;
                    if (!(model instanceof Node)){
                        node = new com.jme.scene.Node(model.getName()+"_node");
                        node.attachChild(model);
                    }else{
                        node = (com.jme.scene.Node)model;
                    }
                    
//                    Callable<Object> exe = new Callable<Object>(){
//                        public Object call(){
//                            model.lockMeshes();
//                            return null;
//                        }
//                    };
//                    JmeContext.get().executeLater(exe);
                    node.setModelBound(new BoundingBox());
                    node.updateModelBound();
                    node.updateWorldBound();
                    node.updateGeometricState(0, true);
//                    BoundingBox box = (BoundingBox) node.getWorldBound();
//                    
//                    node.getLocalTranslation().set(0.0f, box.yExtent / 2.0f, 0.0f);
//                    node.getLocalScale().set(0.02f, 0.02f, 0.02f);
//                    
//                    //ModelLoader.scaleAndCenter(node, scale);
//                    node.updateGeometricState(0, true);
//                    node.lockBounds();
//                    node.lockBranch();
//                    node.lockTransforms();

                    //node.getLocalScale().multLocal(scale);
                    //node.getLocalTranslation().addLocal(0.0f, height, 0.0f);
                    ent.model = node;
                }
                
                entityTypeMap.put(ent.name, ent);
            }
            
            entityNode = entityNode.getNextSibling();
        }
    }
    
    public static Collection<EntityType> getLoadedTypes(){
        return entityTypeMap.values();
    }
    
    public static final com.jme.scene.Node cloneModelForEntity(String typename, String name){
        EntityType type = entityTypeMap.get(typename);
        if (type == null)
            return null;
     
        com.jme.scene.Node node = (com.jme.scene.Node) ModelCloneUtil.cloneSmart(type.model);
        node.setName(name);
        node.updateModelBound();
        node.updateWorldBound();
        node.updateGeometricState(0, true);
        
        return node;
    }
    
    public static Entity realize(Node rootEntityNode){
        return new Entity(rootEntityNode);
    }
    
    public static Entity produce(String typename, String name){
        EntityType type = entityTypeMap.get(typename);
        if (type == null)
            return null;
        
        Entity ent = new Entity(name, type.name);
           
        if (type.model != null){
//            SharedNode node = new SharedNode(name, type.model);
//            node.setModelBound(new BoundingBox());
            com.jme.scene.Node node = (com.jme.scene.Node) ModelCloneUtil.cloneSmart(type.model);
            node.setName(name);
            node.updateModelBound();
            node.updateWorldBound();
            node.updateGeometricState(0, true);
            
            String shortModelName = new File(type.modelName).getName();
            ModelUnit model = new ModelUnit(node, shortModelName);
            ent.attachUnit(model);
        }
        
        return ent;
    }
    
    public static void loadEntityTypes(InputStream xmlTypes){
        try {
            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document doc = builder.parse(xmlTypes);
            
            Node entitiesNode = doc.getElementsByTagName("entities").item(0);
            loadEntityTypes(entitiesNode);
        } catch (SAXException ex) {
            ErrorHandler.reportError("Error while parsing XML file", ex);
        } catch (IOException ex) {
            ErrorHandler.reportError("IO Error while reading input stream", ex);
        } catch (ParserConfigurationException ex) {
            ErrorHandler.reportError("Parser error", ex);
        }
    }
    
    public static void loadDefaultEntityTypes() throws IOException{
        URL url = ResourceLocatorTool.locateResource("entity", "entities.xml");
        InputStream in = url.openStream();
        loadEntityTypes(in);
        in.close();
    }
    
    private EntityManager(){
    }
    
}
