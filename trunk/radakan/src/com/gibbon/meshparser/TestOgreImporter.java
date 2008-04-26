package com.gibbon.meshparser;

import com.jme.scene.Spatial;
import com.jme.system.dummy.DummyDisplaySystem;
import com.jme.util.export.binary.BinaryExporter;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;


public class TestOgreImporter {

    public static void main(String[] args) throws IOException{
        File root = new File("D:\\Suzanne\\");
//        File f = new File("D:\\TileStore\\WoodenHouse.cld.mesh.xml");
        File f2 = new File("D:\\Suzanne\\Scene.material");
        
        File f = new File("D:\\Suzanne\\Suzanne.mesh.xml");
        //File f2 = new File("E:\\Knight\\knight.material");
        
        new DummyDisplaySystem();
        
        SimpleResourceLocator srl = new SimpleResourceLocator(root.toURI());
        ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, srl);
        ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_MODEL, srl);
        
        MaterialLoader matLoader = new MaterialLoader();
        matLoader.load(new FileInputStream(f2));
        
        OgreLoader loader = new OgreLoader();
        loader.setMaterials(matLoader.getMaterials());
        Spatial spatial = loader.loadModel(f.toURI().toURL(), false);
        
        SceneGraphDump.dump(spatial);
        
        BinaryExporter.getInstance().save(spatial, new File("D:\\Dumb.jme"));
    }
    
}
