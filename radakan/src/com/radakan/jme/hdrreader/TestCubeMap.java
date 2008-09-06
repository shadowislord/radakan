package com.radakan.jme.hdrreader;

import java.io.IOException;
import java.net.MalformedURLException;
import java.nio.FloatBuffer;

import com.jme.app.SimpleGame;
import com.jme.bounding.BoundingBox;
import com.jme.image.Image;
import com.jme.image.Texture;
import com.jme.image.TextureCubeMap;
import com.jme.image.Texture.ApplyMode;
import com.jme.image.Texture.CombinerFunctionRGB;
import com.jme.image.Texture.EnvironmentalMapMode;
import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MinificationFilter;
import com.jme.image.Texture.WrapMode;
import com.jme.image.Texture2D;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.Renderer;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.TexCoords;
import com.jme.scene.Spatial.CullHint;
import com.jme.scene.Spatial.LightCombineMode;
import com.jme.scene.TriMesh;
import com.jme.scene.shape.Quad;
import com.jme.scene.shape.Sphere;
import com.jme.scene.shape.Torus;
import com.jme.scene.state.TextureState;
import com.jme.scene.state.ZBufferState;
import com.jme.util.TextureManager;
import com.jme.util.geom.BufferUtils;
import java.io.File;
import java.io.FileInputStream;
import java.util.logging.ConsoleHandler;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.LogRecord;
import java.util.logging.Logger;
import jmetest.renderer.TestBoxColor;

/**
 * <code>TestCubeMap</code>
 * 
 * @author Joshua Slack
 * @version $Id: $
 */
public class TestCubeMap extends SimpleGame {
    
    private Quaternion rotQuat = new Quaternion();
    private float angle = 0;
    private Vector3f axis = new Vector3f(1, 1, 0);
    private TriMesh t;
    //private Node sky;

    /**
     * Entry point for the test,
     * 
     * @param args
     */
    public static void main(String[] args) {
        //TextureManager.registerHandler(".dds", new DDSLoader());
        TestCubeMap app = new TestCubeMap();
        app.setConfigShowMode(ConfigShowMode.AlwaysShow);
        app.start();
    }

    protected void simpleUpdate() {
        if (tpf < 1) {
            angle = angle + (tpf * 25);
            if (angle > 360) {
                angle = 0;
            }
        }

        rotQuat.fromAngleNormalAxis(angle * FastMath.DEG_TO_RAD, axis);
        t.setLocalRotation(rotQuat);
    }

    protected void simpleInitGame() {
        
        //t = new com.jme.scene.shape.Teapot("Teapot");
        t = new Torus("Torus", 50, 50, 5, 10);
        //t = new Sphere("Sphere", 50, 50, 5f);
        t.setModelBound(new BoundingBox());
        t.updateModelBound();
        t.setCullHint(CullHint.Dynamic);
        t.copyTextureCoordinates(0, 1, 1.0f);
        t.scaleTextureCoordinates(0, 8);
        rootNode.attachChild(t);

        TextureState ts = display.getRenderer().createTextureState();
        ts.setTexture(generateCubeMapTexture());
        ts.getTexture().setEnvironmentalMapMode(EnvironmentalMapMode.ReflectionMap);
        ts.getTexture().setApply(ApplyMode.Combine);
        ts.getTexture().setCombineFuncRGB(CombinerFunctionRGB.AddSigned);
        t.setRenderState(ts);
        
        rootNode.setCullHint(CullHint.Never);
        rootNode.setRenderQueueMode(Renderer.QUEUE_OPAQUE);

    }

    private Texture generateCubeMapTexture() {
        Logger.getLogger("").setLevel(Level.FINEST);
        File f = new File("D:\\Snow4.dds");
        
        try {
            Image img = com.jme.image.util.DDSLoader.loadImage(new FileInputStream(f), false);
            System.out.println("Image");
            System.out.println("Resolution: "+img.getWidth()+"x"+img.getHeight());
            System.out.println("Format: "+img.getFormat());
            System.out.println("HasMips: "+img.hasMipmaps());
            //System.out.println("Num Mips: "+img.getMipMapSizes().length);
            //Texture cubeMapImage = TextureManager.loadTexture(f.toURI().toURL());

            Texture cubeMapTex = new TextureCubeMap();
            cubeMapTex.setImage(img);
            cubeMapTex.setMinificationFilter(MinificationFilter.NearestNeighborLinearMipMap);
            cubeMapTex.setMagnificationFilter(MagnificationFilter.Bilinear);
            cubeMapTex.setWrap(WrapMode.EdgeClamp);
            //cubeMapTex.setWrap(WrapMode.Repeat);
            return cubeMapTex;
        } catch (IOException ex) {
            Logger.getLogger(TestCubeMap.class.getName()).log(Level.SEVERE, null, ex);
        } 
        
        return null;
    }

}
