package com.gibbon.radakan.entity.unit;

import com.gibbon.radakan.entity.Entity;
import com.gibbon.radakan.res.ResourceManager;
import com.gibbon.tools.world.EditorUnit;
import com.gibbon.tools.world.World;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import java.io.IOException;

/**
 * ModelUnit is a Unit implementation that allows
 * entities to have a model representation in the world.
 * 
 * @author Momoko_Fan
 */
public class ModelUnit extends AbstractUnit implements UnitEventListener {

    private transient Node model;
    private String modelName;
    
    public ModelUnit(Node model){
        this.model = model;
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
