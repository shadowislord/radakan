package com.radakan.entity.unit;

import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import java.io.IOException;

import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.radakan.entity.Entity;
import com.radakan.entity.EntityFactory;
import com.radakan.res.ResourceManager;
import com.radakan.util.XMLUtil;
import java.io.PrintStream;

/**
 * ModelUnit is a Unit implementation that allows
 * entities to have a model representation in the world.
 * 
 * @author Momoko_Fan
 */
public class ModelUnit extends AbstractUnit implements IUnitEventListener {

    private transient Node model;
    private String modelName;
    
    public ModelUnit(Node model, String modelName){
        this.model = model;
        this.modelName = modelName;
    }
    
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
    
    public void importXML(org.w3c.dom.Node rootEntityNode) {
        org.w3c.dom.Node modelNode = XMLUtil.getChildNode(rootEntityNode, "ModelUnit");
        
        modelName = XMLUtil.getAttribute(modelNode, "filename");
        model = EntityFactory.getInstance().cloneModelForEntity(entity.getType(), entity.getName());
        
        // parse transforms
        model.setLocalTranslation(XMLUtil.getVec3Attribute(modelNode, "translation"));
        model.setLocalScale(XMLUtil.getVec3Attribute(modelNode, "scale"));
        model.setLocalRotation(XMLUtil.getQuatAttribute(modelNode, "rotation"));
    }
    
    @Override
    public void attach(Entity entity){
        super.attach(entity);
        model.setUserData("Entity", entity);
        entity.addEventListener(this);
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
    
    public void onUnitEvent(UnitEvent event) {
        EntityUnit editor = entity.getUnit(EntityUnit.class);
        
        if (event.getType().equals(UnitEvent.ENTITY_BIRTH)){
            // add entity to world rootNode
            //if (editor != null){
            //    World.getWorld().attachModel(model);
            //}
        }else if (event.getType().equals(UnitEvent.ENTITY_DISPOSE)){
            // remove entity from world
            if (editor != null){
                model.removeFromParent();
            }
        }
    }

}
