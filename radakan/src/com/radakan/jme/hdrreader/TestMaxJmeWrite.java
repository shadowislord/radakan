package com.radakan.jme.hdrreader;

import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.jme.app.SimpleGame;
import com.jme.image.Image;
import com.jme.image.Image.Format;
import com.jme.image.Texture;
import com.jme.image.Texture.ApplyMode;
import com.jme.image.Texture.CombinerFunctionRGB;
import com.jme.image.Texture.CombinerSource;
import com.jme.image.Texture.EnvironmentalMapMode;
import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MinificationFilter;
import com.jme.image.Texture.WrapMode;
import com.jme.image.Texture2D;
import com.jme.image.TextureCubeMap;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import com.jme.scene.VBOInfo;
import com.jme.scene.shape.Sphere;
import com.jme.scene.state.TextureState;
import com.jme.util.TextureManager;
import com.jme.util.export.binary.BinaryImporter;
import com.jme.util.geom.BufferUtils;
import com.jmex.model.converters.MaxToJme;
import com.jmex.model.converters.ObjToJme;
import java.nio.ByteBuffer;

/**
 * Started Date: Jun 26, 2004<br>
 * <br>
 * This class test the ability to save adn write .3ds files
 * 
 * @author Jack Lindamood
 */
public class TestMaxJmeWrite extends SimpleGame {
    private static final Logger logger = Logger.getLogger(TestMaxJmeWrite.class
            .getName());
    
    public static void main(String[] args) {
        //TextureManager.registerHandler(".dds", new DDSLoader());
        
        TestMaxJmeWrite app = new TestMaxJmeWrite();
        if (args.length > 0) {
            app.setModelToLoad(args[0]);
        }
        app.setConfigShowMode(ConfigShowMode.AlwaysShow);
        app.start();
    }

    private URL modelToLoad = null;

    private void setModelToLoad(String string) {
        try {
            modelToLoad = (new File(string)).toURI().toURL();
        } catch (MalformedURLException e) {
        }
    }

    private static void setVBO(Spatial s){
        if (s instanceof Node){
            Node n= (Node)s;
            for (Spatial spat : n.getChildren()) setVBO(spat);
        }else{
            TriMesh mesh = (TriMesh) s;
            mesh.setTextureCoords(mesh.getTextureCoords(0), 1);
            mesh.setTextureCoords(mesh.getTextureCoords(0), 2);
            mesh.setTextureCoords(mesh.getTextureCoords(0), 3);
            mesh.setVBOInfo(new VBOInfo(true));
        }
    }
    
    private Texture generateCubeMapTexture() {
        try {
            Texture cubeMapImage = TextureManager.loadTexture(new File("D:\\Snow2.dds").toURI().toURL(), false);

            Texture cubeMapTex = new TextureCubeMap();
            cubeMapTex.setImage(cubeMapImage.getImage());
            cubeMapTex.setMinificationFilter(MinificationFilter.NearestNeighborNoMipMaps);
            cubeMapTex.setMagnificationFilter(MagnificationFilter.Bilinear);
            //cubeMapTex.setWrap(WrapMode.EdgeClamp);
            cubeMapTex.setWrap(WrapMode.Repeat);
            return cubeMapTex;
        } catch (MalformedURLException ex) {
            Logger.getLogger(TestCubeMap.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        return null;
    }
    
    private Texture generateAOTex(){
        try {
            return TextureManager.loadTexture(new File("D:\\AmbientOcclusion_1024.dds").toURI().toURL(), false);
        } catch (MalformedURLException ex) {
            Logger.getLogger(TestMaxJmeWrite.class.getName()).log(Level.SEVERE, null, ex);
        }
        return null;
    }
    
    private Texture generateNilTex(){
        ByteBuffer nilData = BufferUtils.createByteBuffer(4 * 4 * 1);
        Image img = new Image(Format.Luminance8, 4 , 4, nilData);
        Texture t = new Texture2D();
        t.setImage(img);
        return t;
    }
    
    private Texture generateMarbleTex(){
        try {
            return TextureManager.loadTexture(new File("E:\\MarbleWhite.png").toURI().toURL());
        } catch (MalformedURLException ex) {
            Logger.getLogger(TestMaxJmeWrite.class.getName()).log(Level.SEVERE, null, ex);
        }
        return null;
    }
    
    protected void simpleInitGame() {
        URL mtl = null;
        
        if (modelToLoad == null) {
            try {
                modelToLoad = new File("C:\\Hebe.obj").toURI().toURL();
                mtl = new File("C:\\Hebe.mtl").toURI().toURL();
            } catch (MalformedURLException ex) {
                Logger.getLogger(TestMaxJmeWrite.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        try {
            ObjToJme C1 = new ObjToJme();
            C1.setProperty("mtllib", mtl);
            ByteArrayOutputStream BO = new ByteArrayOutputStream();
            C1.convert(new BufferedInputStream(modelToLoad.openStream()), BO);
            Spatial r1 = (Spatial) BinaryImporter.getInstance().load(new ByteArrayInputStream(BO.toByteArray()));
            //r1 = new Sphere("Sphere", 20, 20, 1f);
            //Node r = new Node("parent stuff");
            //r.attachChild(C1.get(new BufferedInputStream(modelToLoad.openStream()), BO));
            //r.setLocalScale(.1f);
            r1.setLocalScale(5.0f);
           
//            Quaternion temp = new Quaternion();
//            temp.fromAngleAxis(FastMath.PI / 2, new Vector3f(-1, 0, 0));
//            rootNode.setLocalRotation(temp);
//            r1.setLocalTranslation(new Vector3f(10,0,0));
            //rootNode.attachChild(r);
            
            
            TextureState ts = display.getRenderer().createTextureState();

            Texture t0 = generateCubeMapTexture();
            t0.setApply(ApplyMode.Combine);
            t0.setCombineFuncRGB(CombinerFunctionRGB.Modulate);
            t0.setCombineSrc1RGB(CombinerSource.Constant);
            t0.setBlendColor(new ColorRGBA(0.1f, 0.1f, 0.1f, 1.0f));
            t0.setEnvironmentalMapMode(EnvironmentalMapMode.ReflectionMap);
            ts.setTexture(t0, 0);
            
            Texture t1 = generateNilTex();
            t1.setApply(ApplyMode.Combine);
            t1.setCombineFuncRGB(CombinerFunctionRGB.Add);
            t1.setCombineSrc0RGB(CombinerSource.PrimaryColor);
            t1.setCombineSrc1RGB(CombinerSource.Previous);
            ts.setTexture(t1, 1);
            
            Texture t2 = generateMarbleTex();
            t2.setApply(ApplyMode.Modulate);
            ts.setTexture(t2, 2);

            Texture t3 = generateAOTex();
            t3.setApply(ApplyMode.Combine);
            t3.setCombineFuncRGB(CombinerFunctionRGB.AddSigned);
            ts.setTexture(t3, 3);
            
//            Texture t1 = generateCubeMapTexture();
//            t1.setApply(ApplyMode.Combine);
//            t1.setCombineFuncRGB(CombinerFunctionRGB.AddSigned);
//            t1.setEnvironmentalMapMode(EnvironmentalMapMode.ReflectionMap);
//            ts.setTexture(t1, 1);
            
//            Texture t2 = generateAOTex();
//            t2.setApply(ApplyMode.Combine);
//            t2.setCombineFuncRGB(CombinerFunctionRGB.AddSigned);
//            ts.setTexture(t2, 1);
            
            
            //t1.setApply(ApplyMode.Replace);
            
            
            r1.setRenderState(ts);
            setVBO(r1);
            
            rootNode.attachChild(r1);
            rootNode.updateRenderState();
            rootNode.updateGeometricState(0f, true);
        } catch (IOException e) {
            logger.log(Level.SEVERE, "Failed to load Max file", e);
        }
    }
}