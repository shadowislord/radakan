package com.radakan.entity.unit;

import java.io.IOException;
import java.io.PrintStream;

import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import org.w3c.dom.Node;

public class PlayerBehaviorUnit extends AbstractUnit {

    @Override
    public void read(JMEImporter im) throws IOException {
    // TODO Auto-generated method stub

    }

    @Override
    public void update(float tpf) {


    }

    @Override
    public void write(JMEExporter ex) throws IOException {
    // TODO Auto-generated method stub

    }

    @Override
    public void exportXML(PrintStream stream) {
    // TODO Auto-generated method stub

    }

    public void importXML(Node rootEntityNode) {
        throw new UnsupportedOperationException("Not supported yet.");
    }
}
