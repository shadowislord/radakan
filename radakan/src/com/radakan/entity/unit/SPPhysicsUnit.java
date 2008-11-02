package com.radakan.entity.unit;

import com.jme.bounding.BoundingBox;
import com.jme.math.Vector3f;
import com.jme.scene.Spatial;
import com.jme.util.export.InputCapsule;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.jme.util.export.OutputCapsule;
import java.io.IOException;
import java.io.PrintStream;
import org.w3c.dom.Node;
import com.jmex.simplephysics.*;
import com.radakan.util.XMLUtil;


public class SPPhysicsUnit extends Unit implements IPhysicsUnit {
    
    private float resistance = 0.05f;
    private float mass = 1;
    private float bounciness = 0;
    private boolean bounceEnabled = true;
    private float slopeFactor = 1;
    private Vector3f curForce;
    
    private DynamicCollider dc;
    
    @Override
    public void addForce(Vector3f force) {
        curForce.addLocal(force);
        dc.getPhysicState().addForce(force);
    }

    @Override
    public void setForce(Vector3f force) {
        curForce.set(force);
        dc.getPhysicState().setForce(force);
    }

    @Override
    public Vector3f getForce() {
        return curForce;
    }

    public DynamicCollider getCollider(){
        return dc;
    }
    
    @Override
    public void update(float tpf) {
        // Physics state updated by PhysicsUpdatePass
    }

    @Override
    public void write(JMEExporter ex) throws IOException {
        OutputCapsule oc = ex.getCapsule(this);
        oc.write(resistance, "resistance", 0.05f);
        oc.write(mass, "mass", 1f);
        oc.write(bounciness, "bounciness", 0f);
        oc.write(bounceEnabled, "bounceEnabled", true);
        oc.write(slopeFactor, "slopeFactor", 1f);
    }

    @Override
    public void read(JMEImporter im) throws IOException {
        InputCapsule ic = im.getCapsule(this);
        resistance = ic.readFloat("resistance", 0.05f);
        mass = ic.readFloat("mass", 1f);
        bounciness = ic.readFloat("bounciness", 0f);
        bounceEnabled = ic.readBoolean("bounceEnabled", true);
        slopeFactor = ic.readFloat("slopeFactor", 1f);
    }

    @Override
    public void exportXML(PrintStream stream) {
        stream.println("    <PhysicsUnit resistance=\"" + resistance + "\"");
        stream.println("                 mass=\"" + mass + "\"");
        stream.println("                 bounciness=\"" + bounciness + "\"");
        stream.println("                 slopeFactor=\"" + slopeFactor + "\"");
        stream.println("                 bounceEnabled=\"" + bounceEnabled + "\" />");
    }

    @Override
    public void importXML(Node rootPhysicsNode) {
        if (rootPhysicsNode == null)
            throw new IllegalStateException("Cannot find PhysicsUnit attribute in "+entity.getName());
        
        resistance    = XMLUtil.getFloatAttribute(rootPhysicsNode, "resistance");
        mass          = XMLUtil.getFloatAttribute(rootPhysicsNode, "mass");
        bounciness    = XMLUtil.getFloatAttribute(rootPhysicsNode, "bounciness");
        slopeFactor   = XMLUtil.getFloatAttribute(rootPhysicsNode, "slopeFactor");
        bounceEnabled = XMLUtil.getBoolAttribute(rootPhysicsNode, "bounceEnabled");
    }

    @Override
    public int getType() {
        return PHYSICS;
    }

    @Override
    public void onUnitEvent(UnitEvent event) {
        if (event.getType().equals(UnitEvent.ENTITY_BIRTH)){
            Spatial s = entity.getModel().getModel();
            BoundingBox bbox = (BoundingBox) s.getWorldBound();
            
            Vector3f dimensions = new Vector3f(bbox.xExtent * 2f, bbox.yExtent * 2f, bbox.zExtent * 2f);
            dc = new EllipseCollider(s.getWorldTranslation(), dimensions, s);
            dc.getPhysicMaterial().set(resistance, mass, slopeFactor, bounciness, bounceEnabled);
        }
    }

}
