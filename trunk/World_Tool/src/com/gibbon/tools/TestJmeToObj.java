package com.gibbon.tools;

import com.jme.math.Vector3f;
import com.jme.scene.Node;
import com.jme.scene.shape.Box;
import com.jme.scene.shape.Pyramid;
import java.io.File;
import java.io.IOException;

public class TestJmeToObj {

    public static void main(String[] args) throws IOException{
        
        Node n = new Node("MyNode");
        
        Box b= new Box("BOX", Vector3f.UNIT_XYZ.negate(), Vector3f.UNIT_XYZ);
        Pyramid p = new Pyramid("Pyramid", 2f, 2f);
        
        //p.setLocalTranslation(-10, 0, 0);
        
        n.attachChild(p);
        n.attachChild(b);
        
        JmeToObj converter = new JmeToObj();
        converter.convert(n, new File("C:\\"));
        
    }
    
}
