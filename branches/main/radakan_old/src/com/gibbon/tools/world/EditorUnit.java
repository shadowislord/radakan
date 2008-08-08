package com.gibbon.tools.world;

import com.gibbon.radakan.entity.EntityFactory.EntityType;
import com.gibbon.radakan.entity.unit.AbstractUnit;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import java.io.IOException;
import java.io.PrintStream;

public class EditorUnit extends AbstractUnit {

    private final EntityType type;
    
    public EditorUnit(EntityType type){
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

    public void exportXML(PrintStream stream) {
        stream.append("<editor type=\""+type.name+"\" />");
    }

}
