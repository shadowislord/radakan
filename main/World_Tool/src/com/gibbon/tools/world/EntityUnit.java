package com.gibbon.tools.world;

import com.gibbon.radakan.entity.EntityFactory.EntityType;
import com.gibbon.radakan.entity.unit.AbstractUnit;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import java.io.IOException;

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
