package com.gibbon.tools;

import com.jme.bounding.BoundingBox;
import com.jme.bounding.BoundingVolume;
import com.jme.math.Vector3f;
import com.jme.scene.Spatial;
import com.jme.util.export.binary.BinaryExporter;
import com.jme.util.export.binary.BinaryImporter;
import com.jme.util.export.xml.XMLExporter;
import com.jme.util.export.xml.XMLImporter;
import com.jme.util.resource.MultiFormatResourceLocator;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import com.jmex.model.converters.*;

//import com.model.md5.importer.MD5Importer;
import com.radakan.graphics.mesh.parser.Material;
import com.radakan.graphics.mesh.parser.MaterialLoader;
import com.radakan.graphics.mesh.parser.OgreLoader;
import com.radakan.graphics.mesh.parser.SceneLoader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.HashMap;
import java.util.Map;
import javax.swing.JOptionPane;


public final class ModelLoader {

    private ModelLoader() {
    }
    
    public static Spatial loadModel(File file, FormatConverter converter) throws IOException{
        InputStream in = new FileInputStream(file);
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        converter.convert(in, out);
        return (Spatial) BinaryImporter.getInstance().load(out.toByteArray());
    }
    
    /*public static Spatial loadMD5Model(File file) throws IOException{
        MD5Importer im = MD5Importer.getInstance();

        String name = file.getName();
        int i = name.lastIndexOf(".");
        if (i > 0) name = name.substring(0, i);

        URL mesh = file.toURI().toURL();
        URL anim = null;

        file = new File(file.getParentFile(), name + ".md5anim");
        if (file.exists()){
            anim = file.toURI().toURL();
            im.load(mesh, name, anim, name + "_anim", Controller.RT_WRAP);
        }else{
            im.loadMesh(mesh, name);
        }

        return (Spatial) im.getModelNode();
    }*/
    
//    public static Spatial loadDAEModel(File file) throws IOException{
//        InputStream in = new FileInputStream(file);
//        ColladaImporter.load( in , "Model" );
//        Spatial s = ColladaImporter.getModel();
//        in.close();
//        ColladaImporter.cleanUp();
//        return s;
//    }
    
    public static Spatial loadMeshModel(File file) throws IOException{
        Map<String, Material> materials = new HashMap<String, Material>();
        
        // scan for material files
        File parent = file.getParentFile();
        for (File f : parent.listFiles()){
            if (f.getName().endsWith(".material")){
                MaterialLoader matloader = new MaterialLoader();
                InputStream in = new FileInputStream(f);
                matloader.load(in);
                in.close();
                materials.putAll(matloader.getMaterials());
            }
        }
        
        OgreLoader loader = new OgreLoader();
        loader.setMaterials(materials);
        return loader.loadModel(file.toURI().toURL(), false);
    }
    
    public static Spatial loadDotScene(File file) throws IOException{
        SceneLoader loader = new SceneLoader();
        FileInputStream in = new FileInputStream(file);
        loader.load(in);
        in.close();
        return loader.getScene();
    }
    
    public static Spatial scaleAndCenter(Spatial model, float size) {
        if (model != null) {
            model.updateGeometricState(0, true);
            
            BoundingVolume worldBound = model.getWorldBound();
            if (worldBound == null) {
                model.setModelBound(new BoundingBox());
                model.updateModelBound();
                model.updateGeometricState(0, true);
                worldBound = model.getWorldBound();
            }
            
            if (worldBound != null){ // check not still null (no geoms)
                Vector3f center = worldBound.getCenter();
                
                BoundingBox boundingBox = new BoundingBox(center, 0, 0, 0);
                boundingBox.mergeLocal(worldBound);
                
                Vector3f extent = boundingBox.getExtent( null );
                float maxExtent = Math.max( Math.max( extent.x, extent.y ), extent.z );
                float height = extent.y;
                if ( maxExtent != 0 ) {
                    model.setLocalScale( size / maxExtent );
                    Vector3f pos = center.negate().addLocal(0.0f, height / 2.0f, 0.0f); //.multLocal(model.getLocalScale().x);
                    model.setLocalTranslation(pos);
                    System.out.println("Model size: "+maxExtent);
                    System.out.println("Model position: "+center);
                }
            }
        }
        return model;
    }
    
    public static void saveModel(Spatial model, File file, String ext) throws IOException{
        if (ext.equalsIgnoreCase("jme")){
            BinaryExporter.getInstance().save(model, file);
        }else if (ext.equalsIgnoreCase("xml")){
            OutputStream out = new FileOutputStream(file);
            XMLExporter.getInstance().save(model, out);
            out.close();
        }else if (ext.equalsIgnoreCase("obj")){
            JmeToObj convert = new JmeToObj();
            convert.convert(model, file.getParentFile());
        }else{
            JOptionPane.showMessageDialog(
                                     null,
                                     "Error: Format not supported: "+ext,
                                     "Error",
                                     JOptionPane.ERROR_MESSAGE);
        }
    }
    
    public static Spatial loadModel(File file, String ext) throws IOException{
        MultiFormatResourceLocator locator 
                = new MultiFormatResourceLocator(file.getParentFile().toURI(), "png", "dds", "tga", "bmp", "jpg", "wbmp", "gif");
        locator.setTrySpecifiedFormatFirst(true);
        ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, locator);
        
        SimpleResourceLocator locator2 = new SimpleResourceLocator(file.getParentFile().toURI());
        ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_MODEL, locator2);
        
        FormatConverter converter = null;
        if (ext.equalsIgnoreCase("md3")){
            converter = new Md3ToJme();
        }else if (ext.equalsIgnoreCase("obj")){
            converter = new ObjToJme();
            converter.setProperty("texdir", file.getParent().toString());
            converter.setProperty("mtllib", file.getParent().toString());
        }else if (ext.equalsIgnoreCase("ase")){
            converter = new AseToJme();
        }else if (ext.equalsIgnoreCase("3ds")){
            converter = new MaxToJme();
        }else if (ext.equalsIgnoreCase("ms3d")){
            converter = new MilkToJme();
        }else if (ext.equalsIgnoreCase("md2")){
            converter = new Md2ToJme();
        }else if (ext.equalsIgnoreCase("x3d")){
            try {
                converter = new X3dToJme();
                converter.setProperty("textures", file.getParent().toString());
            } catch (InstantiationException ex) {
                ex.printStackTrace();
            }
        }
        
        Spatial model = null;
        
        if (converter != null){
            model = loadModel(file, converter);
        }else if (ext.equalsIgnoreCase("md5mesh")){
            //model = loadMD5Model(file);
        }else if (ext.equalsIgnoreCase("dae")){
//            model = loadDAEModel(file);
        }else if (ext.equalsIgnoreCase("mesh.xml")){
            model = loadMeshModel(file);
        }else if (ext.equalsIgnoreCase("scene")){
            model = loadDotScene(file);
        }else if (ext.equalsIgnoreCase("xml")){
            // jme XML
            model = (Spatial) XMLImporter.getInstance().load(file);
        }else{
            JOptionPane.showMessageDialog(
                             null,
                             "Error: Format not supported: "+ext,
                             "Error",
                             JOptionPane.ERROR_MESSAGE);
            return null;
        }
        
        ResourceLocatorTool.removeResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, locator);
        ResourceLocatorTool.removeResourceLocator(ResourceLocatorTool.TYPE_MODEL, locator2);
        
        return model;
    }

}
