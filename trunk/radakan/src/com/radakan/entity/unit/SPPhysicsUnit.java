package com.radakan.entity.unit;

import com.jme.math.Vector3f;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.radakan.entity.Entity;
import java.io.IOException;
import java.io.PrintStream;
import org.w3c.dom.Node;

public class SPPhysicsUnit implements IPhysicsUnit {

    @Override
    public void addForce(Vector3f force) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public void setForce(Vector3f force) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public void getForce(Vector3f force) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public void exportXML(PrintStream stream) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public void importXML(Node rootEntityNode) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public void update(float tpf) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public void attach(Entity entity) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public void detach() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public int getType() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public void write(JMEExporter ex) throws IOException {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public void read(JMEImporter im) throws IOException {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public Class getClassTag() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

}
