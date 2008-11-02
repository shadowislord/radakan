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

package com.radakan.game.entity.unit;

import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.scene.Controller;
import java.io.IOException;

import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.radakan.game.Game;
import com.radakan.game.entity.Entity;
import com.radakan.game.entity.GameEntityManager;
import com.radakan.jme.mxml.anim.MeshAnimationController;
import com.radakan.res.ResourceManager;
import com.radakan.util.XMLUtil;
import java.io.PrintStream;

/**
 * ModelUnit is a Unit implementation that allows
 * entities to have a model representation in the world.
 * 
 * @author Momoko_Fan
 */
public class ModelUnit extends Unit{

    private transient Node model;
    private String modelName;
    
    /**
     * Given a jME scene graph node and model name, creates a ModelUnit.
     * 
     * @param model jME node containing the model to be rendered.
     * @param modelName Name of the model file from which the model was loaded,
     * required if the entity is to be exported later on.
     */
    public ModelUnit(Node model, String modelName){
        this.model = model;
        this.modelName = modelName;
    }
    
    /**
     * Empty constructor for serialization.
     * importXML will be used to initialize fields.
     */
    public ModelUnit(){
    }
    
    /**
     * Export the model name and transforms to XML.
     * @param stream
     */
    @Override
    public void exportXML(PrintStream stream) {
        Vector3f pos = model.getLocalTranslation();
        Quaternion rot = model.getLocalRotation();
        Vector3f scale = model.getLocalScale();
        
        stream.println("    <ModelUnit filename=\"" + modelName + "\"");
        stream.println("               translation=\"" + pos.x + ", " + pos.y + ", " + pos.z + "\"");
        stream.println("               rotation=\"" + rot.x + ", " + rot.y + ", " + rot.z + ", " + rot.w + "\"");
        stream.println("               scale=\"" + scale.x + ", " + scale.y + ", " + scale.z + "\" />");
        //stream.println("    </model>");
    }
    
    /**
     * Imports a model unit that was exported with exportXML().
     * @param rootModelNode
     */
    @Override
    public void importXML(org.w3c.dom.Node rootModelNode) {
        if (rootModelNode == null)
            throw new IllegalStateException("Cannot find ModelUnit attribute in "+entity.getName());
        
        GameEntityManager entityMan = Game.getEntityManager();
        
        model = entityMan.cloneModelForEntity(entity.getType(), entity.getName());
        modelName = entityMan.getEntityType(entity.getType()).modelName;
        
        if (model == null)
            return;
        
        // parse transforms
        model.setLocalTranslation(XMLUtil.getVec3Attribute(rootModelNode, "translation"));
        model.setLocalScale(XMLUtil.getVec3Attribute(rootModelNode, "scale"));
        model.setLocalRotation(XMLUtil.getQuatAttribute(rootModelNode, "rotation"));
        
        model.setUserData("Entity", entity);
    }
    
    @Override
    public void attach(Entity entity){
        super.attach(entity);
    }
    
    @Override
    public void update(float tpf) {
    }

    @Override
    public void write(JMEExporter ex) throws IOException {
        ex.getCapsule(this).write(modelName, "modelName", "");
    }

    @Override
    public void read(JMEImporter im) throws IOException {
        modelName = im.getCapsule(this).readString(modelName, "");
        model = (Node) ResourceManager.loadResource(Spatial.class, modelName);
    }

    public Spatial getModel(){
        return model;
    }
    
    @Override
    public void onUnitEvent(UnitEvent event) {
        //EntityUnit editor = entity.getUnit(EntityUnit.class);
        
        if (event.getType().equals(UnitEvent.ENTITY_BIRTH)){
            // add entity to world rootNode
            //if (editor != null){
            //    World.getWorld().attachModel(model);
            //}
            if (model != null){
                model.setUserData("Entity", entity);

                if (model.getControllerCount() > 0){
                    Controller c = model.getController(0);
                    if (c instanceof MeshAnimationController){
                        OgreAnimationUnit ogreAnim = new OgreAnimationUnit((MeshAnimationController)c);
                        entity.attachUnit(ogreAnim);
                    }
                }
            }
        }else if (event.getType().equals(UnitEvent.ENTITY_DISPOSE)){
            // remove entity from world
            //if (editor != null){
            //    model.removeFromParent();
            //}
        }
    }

    /**
     * @return IUnit.MODEL
     */
    @Override
    public int getType() {
        return MODEL;
    }

}
