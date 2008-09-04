package com.radakan.util.preview;

import com.jme.math.Vector3f;
import com.jme.scene.Node;
import com.jme.scene.shape.Box;
import com.jme.scene.shape.Pyramid;
import com.jme.scene.shape.Teapot;
import java.io.File;
import java.io.IOException;

public class TestJmeToObj {

    public static void main(String[] args) throws IOException{
        Teapot teaPot = new Teapot("Ah, I forgot to initialize it");
        JmeToObj converter = new JmeToObj();
        converter.convert(teaPot, new File("C:\\"));
    }
    
}
