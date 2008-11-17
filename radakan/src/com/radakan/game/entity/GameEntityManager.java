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

package com.radakan.game.entity;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.xml.sax.SAXException;

import com.jme.bounding.BoundingBox;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.scene.Spatial;
import com.jme.util.resource.ResourceLocatorTool;
import com.radakan.game.Game;
import com.radakan.game.entity.unit.ModelUnit;
import com.radakan.jme.mxml.anim.MeshAnimationController;
import com.radakan.jme.mxml.MeshCloner;
import com.radakan.game.res.ModelCloneUtil;
import com.radakan.game.res.GameResourceManager;
import com.radakan.util.XMLUtil;
import java.net.URL;

/**
 * The EntityManager is a utility class for allowing entity creation, export and import.
 * It loads entity type information from a database, with the name "entities.xml"
 * located in the meta/entity folder. 
 * 
 * @author Kirill Vainer
 */
public class GameEntityManager {

    private final Map<String, EntityType> entityTypeMap = new HashMap<String, EntityType>();
    
    /**
     * An entity type defines parameters of the entities which are of it's type.
     */
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

    /**
     * Loads the "default" entity types. Typically stored in the entities.xml file,
     * inside one of the PAK archives.
     * 
     * Depends on:
     *  - GameResourceManager
     *  - GameDebugManager
     * 
     * @throws java.io.IOException If an I/O error occurs.
     */
    public GameEntityManager(){
        InputStream in = null;
        try {
            URL url = ResourceLocatorTool.locateResource("entity", "entities.xml");
            in = url.openStream();
            loadEntityTypes(in);
            in.close();
        } catch (IOException ex) {
            Game.getDebugManager().reportError("Error while reading entity type data", ex);
        } finally {
            try {
                in.close();
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
    }
    
    /**
     * Returns all loaded EntityType objects. Usuaully after a call to loadEntityTypes().
     * @return Collection of EntityTypes.
     */
    public Collection<EntityType> getLoadedTypes(){
        return entityTypeMap.values();
    }
    
    /**
     * Looks up an EntityType given a type "name". 
     * @param typename The name of the EntityType. Defined as an attribute of the EntityType tag.
     * @return An EntityType with the given typename, or null if not found.
     */
    public final EntityType getEntityType(String typename){
        return entityTypeMap.get(typename);
    }
    
    /**
     * Given an EntityType name, and a model name, creates a unique copy of the model
     * represented by the EntityType. 
     * 
     * @param typename The name of the EntityType. Defined as an attribute of the EntityType tag.
     * @param name Name of the model used for the jME scene graph.
     * @return A node containing the cloned model. 
     */
    public final com.jme.scene.Node cloneModelForEntity(String typename, String name){
        EntityType type = entityTypeMap.get(typename);
        if (type == null)
            return null;
     
        com.jme.scene.Node node;
        if (type.model.getControllerCount() > 0 && type.model.getController(0) instanceof MeshAnimationController){
            node = MeshCloner.cloneMesh(type.model);
        }else{
            node = (com.jme.scene.Node) ModelCloneUtil.cloneSmart(type.model);
        }
        
        node.setName(name);
        node.updateModelBound();
        node.updateWorldBound();
        node.updateGeometricState(0, true);
        
        return node;
    }
    
    /**
     * Loads an Entity given an XML Node. 
     * This method should be used for loading entities from tile.xml files.
     * 
     * @param rootEntityNode An XML node of the entity. It's node name must be "entity"
     * @return The Entity object loaded as defined by the XML node.
     */
    public Entity realize(Node rootEntityNode){
        return new Entity(rootEntityNode);
    }
    
    /**
     * Creates a new Entity from a given type name, and entity name.
     * 
     * @param typename The name of the EntityType. Defined as an attribute of the EntityType tag.
     * @param name Name of the unique entity used for identification and in-game usage purposes.
     * @return The produced entity.
     */
    public Entity produce(String typename, String name){
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
            node.setUserData("Entity", ent);
            
            String shortModelName = new File(type.modelName).getName();
            ModelUnit model = new ModelUnit(node, shortModelName);
            ent.attachUnit(model);
        }
        
        return ent;
    }
    
    /**
     * Loads all EntityTypes in a types.xml file.
     * 
     * @param entitiesNode The root of the types.xml file.
     * @throws java.io.IOException If any I/O error occures while reading models or textures.
     */
    private void loadEntityTypes(Node entitiesNode) throws IOException{
        GameResourceManager resource = Game.getResourceManager();
        
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
                    final Spatial model = resource.loadModel(ent.modelName);
                    com.jme.scene.Node node = null;
                    if (!(model instanceof com.jme.scene.Node)){
                        node = new com.jme.scene.Node(model.getName()+"_node");
                        node.attachChild(model);
                    }else{
                        node = (com.jme.scene.Node)model;
                        for (Spatial child : node.getChildren()){
                            child.setLocalScale(scale);
                            
                            Quaternion rot = new Quaternion();
                            rot.fromAngleAxis(-FastMath.HALF_PI, Vector3f.UNIT_Y);
                            child.setLocalRotation(rot);
                        }
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
    
    /**
     * Loads entity types from the XML file InputStream.
     * @param xmlTypes An input stream to an types.xml file.
     */
    public void loadEntityTypes(InputStream xmlTypes){
        try {
            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document doc = builder.parse(xmlTypes);
            
            Node entitiesNode = doc.getElementsByTagName("entities").item(0);
            loadEntityTypes(entitiesNode);
        } catch (SAXException ex) {
            Game.getDebugManager().reportError("Error while parsing XML file", ex);
        } catch (IOException ex) {
            Game.getDebugManager().reportError("IO Error while reading input stream", ex);
        } catch (ParserConfigurationException ex) {
            Game.getDebugManager().reportError("Parser error", ex);
        }
    }
    
}
