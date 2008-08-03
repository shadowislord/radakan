package com.gibbon.radakan.entity.unit;

import com.gibbon.radakan.entity.Entity;
import com.gibbon.radakan.res.ResourceManager;
import com.gibbon.tools.world.EditorUnit;
import com.gibbon.tools.world.World;
import com.jme.math.Vector3f;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import java.io.IOException;
import java.io.PrintStream;

/**
 * ModelUnit is a Unit implementation that allows
 * entities to have a model representation in the world.
 * 
 * @author Momoko_Fan
 */
public class ModelUnit extends AbstractUnit implements UnitEventListener {

    private transient Node model;
    private String modelName;
    
    private float angle = 0.0f;
    private Vector3f normal;
    
    public ModelUnit(Node model){
        this.model = model;
    }
    
    public float getAngle(){
        return angle;
    }
    
    public Vector3f getNormal(){
        return normal;
    }
    
    public void setAngle(float newAngle){
        angle = newAngle;
        if (normal == null)
            normal = Vector3f.UNIT_Y.clone();
        
        model.getLocalRotation().fromAngleAxis(angle, normal);
        model.updateGeometricState(0, true);
    }
    
    public void setNormal(Vector3f newNormal){
        if (this.normal == null)
            this.normal = Vector3f.UNIT_Y.clone();
        
        this.normal.set(newNormal);
        model.getLocalRotation().fromAngleAxis(angle, normal);
        model.updateGeometricState(0, true);
    }
    
    public Vector3f getPosition(){
        return model.getWorldTranslation();
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

    public void exportXML(PrintStream stream){
        Vector3f pos = getPosition();
        Vector3f norm = getNormal();
        
        stream.println("    <model position=\""+pos.x+", "+pos.y+", "+pos.z+"\"");
        stream.println("       normal=\""+norm.x+", "+norm.y+", "+norm.z+"\"");
        stream.println("       angle=\""+angle+"\"");
        stream.println("    </model>");
    }
    
    public void exportXMLType(PrintStream stream){
        stream.println("    <model filename=\""+modelName+"\"/>");
    }
    
    public Spatial getModel(){
        return model;
    }
    
    public void onUnitEvent(UnitEvent event) {
        EditorUnit editor = entity.getUnit(EditorUnit.class);
        
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
