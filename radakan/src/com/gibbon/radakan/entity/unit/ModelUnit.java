package com.gibbon.radakan.entity.unit;

import com.gibbon.radakan.entity.Entity;
import com.gibbon.radakan.res.ResourceManager;
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
public class ModelUnit extends AbstractUnit {

    private transient Spatial model;
    private String modelName;
    
    public ModelUnit(){
    }
    
    @Override
    public void attach(Entity entity){
        super.attach(entity);
        model.setUserData("Entity", entity);
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
        model = ResourceManager.loadResource(Spatial.class, modelName);
    }
    
}
