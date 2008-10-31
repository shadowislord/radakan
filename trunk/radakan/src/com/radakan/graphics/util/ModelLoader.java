package com.radakan.graphics.util;

import com.jme.bounding.BoundingBox;
import com.jme.bounding.BoundingVolume;
import com.jme.math.Vector3f;
import com.jme.scene.Controller;
import com.jme.scene.Spatial;
import com.jme.util.export.binary.BinaryImporter;
import com.jme.util.resource.MultiFormatResourceLocator;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
//import com.model.md5.importer.MD5Importer;
import com.jmex.model.converters.AseToJme;
import com.jmex.model.converters.FormatConverter;
import com.jmex.model.converters.MaxToJme;
import com.jmex.model.converters.Md2ToJme;
import com.jmex.model.converters.Md3ToJme;
import com.jmex.model.converters.MilkToJme;
import com.jmex.model.converters.ObjToJme;
import com.jmex.model.converters.X3dToJme;
import com.model.md5.importer.MD5Importer;
import com.radakan.jme.mxml.Material;
import com.radakan.jme.mxml.MaterialLoader;
import com.radakan.jme.mxml.OgreLoader;
import com.radakan.jme.mxml.SceneLoader;
import com.radakan.util.ErrorHandler;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;
import javax.swing.JOptionPane;


public final class ModelLoader {

    private ModelLoader() {
    }
    
    public static Spatial loadModel(URL url, FormatConverter converter) throws IOException{
        InputStream in = url.openStream();
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        converter.convert(in, out);
        in.close();
        return (Spatial) BinaryImporter.getInstance().load(out.toByteArray());
    }
    
    public static Spatial loadMD5Model(URL url) throws IOException{
        MD5Importer im = MD5Importer.getInstance();

        String name = url.toString();
        int i = name.lastIndexOf(".");
        if (i > 0)
            name = name.substring(0, i);

        //file = new File(file.getParentFile(), name + ".md5anim");
        //if (file.exists()){
        //    anim = file.toURI().toURL();
        //    im.load(mesh, name, anim, name + "_anim", Controller.RT_WRAP);
        //}else{
            im.loadMesh(url, name);
        //}

        return (Spatial) im.getModelNode();
    }
    
//    public static Spatial loadDAEModel(File file) throws IOException{
//        InputStream in = new FileInputStream(file);
//        ColladaImporter.load( in , "Model" );
//        Spatial s = ColladaImporter.getModel();
//        in.close();
//        ColladaImporter.cleanUp();
//        return s;
//    }
    
    public static Spatial loadMeshModel(URL url, boolean loadMats) throws IOException{
        Map<String, Material> materials = new HashMap<String, Material>();
        
        if (loadMats){
            String name = url.toString();
            if (name.toLowerCase().endsWith(".mesh.xml")){
                name = name.substring(0, name.length() - 9);
            }
            
            
            URL matURL = new URL(name + ".material");
            
            MaterialLoader matloader = new MaterialLoader();
            try{
                InputStream in = matURL.openStream();
                matloader.load(in);
                in.close();
                materials.putAll(matloader.getMaterials());
            } catch (IOException ex){
                if (!(ex instanceof FileNotFoundException)){
                    ex.printStackTrace();
                }
            }
        }
        
        OgreLoader loader = new OgreLoader();
        loader.setMaterials(materials);
        return loader.loadModel(url);
    }
    
    public static Spatial loadDotScene(URL url) throws IOException{
        SceneLoader loader = new SceneLoader();
        InputStream in = url.openStream();
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
    
    public static Spatial loadModel(URL url, String ext) throws IOException{
        String name = url.toString();
        int index = name.lastIndexOf('/');
        name = name.substring(0, index+1);
        URL parent = new URL(name);
        
        MultiFormatResourceLocator locator = null;
        SimpleResourceLocator locator2 = null;
        try{
            locator = new MultiFormatResourceLocator(parent, "png", "dds", "tga", "bmp", "jpg", "wbmp", "gif");
            locator.setTrySpecifiedFormatFirst(true);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, locator);

            locator2 = new SimpleResourceLocator(parent);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_MODEL, locator2);
        } catch (URISyntaxException ex){
            ErrorHandler.reportError("Error while creating locators for model "+url, ex);
        }
        
        FormatConverter converter = null;
        if (ext.equalsIgnoreCase("md3")){
            converter = new Md3ToJme();
        }else if (ext.equalsIgnoreCase("obj")){
            converter = new ObjToJme();
            converter.setProperty("texdir", parent.toString());
            converter.setProperty("mtllib", parent.toString());
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
                converter.setProperty("textures", parent.toString());
            } catch (InstantiationException ex) {
                ex.printStackTrace();
            }
        }
        
        Spatial model = null;
        
        if (converter != null){
            model = loadModel(url, converter);
        }else if (ext.equalsIgnoreCase("md5mesh")){
            //model = loadMD5Model(file);
        }else if (ext.equalsIgnoreCase("dae")){
            //model = loadDAEModel(file);
        }else if (ext.equalsIgnoreCase("mesh.xml")){
            model = loadMeshModel(url, true);
        }else if (ext.equalsIgnoreCase("scene")){
            model = loadDotScene(url);
        }else{
            JOptionPane.showMessageDialog(
                             null,
                             "Error: Format not supported: "+ext,
                             "Error",
                             JOptionPane.ERROR_MESSAGE);
            return null;
        }
        
        ResourceLocatorTool.removeResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, locator);
        //ResourceLocatorTool.removeResourceLocator(ResourceLocatorTool.TYPE_MODEL, locator2);
        
        return model;
    }

}
