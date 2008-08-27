package com.radakan.entity.unit;

import java.io.IOException;

import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.radakan.entity.EntityManager.EntityType;
import java.io.PrintStream;
import org.w3c.dom.Node;

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

    public void exportXML(PrintStream stream) {
        //stream.println("<type name=\""+ type.name + "\" />");
    }

    public void importXML(Node rootEntityNode) {
    }

}
