package com.radakan.entity.unit;

import java.io.IOException;

import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.radakan.entity.EntityFactory.EntityType;

public class EntityUnit extends AbstractUnit {

    private final EntityType type;
    
    public EntityUnit(EntityType type){
        this.type = type;
    }
    
    public EntityType getType(){
        return type;
    }
    
    @Override
    public void update(float tpf) {
    }

    @Override
    public void write(JMEExporter ex) throws IOException {
    }

    @Override
    public void read(JMEImporter im) throws IOException {
    }

}
