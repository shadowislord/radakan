package com.radakan.graphics.mesh.parser;

import com.jme.app.SimpleGame;
import com.jme.input.FirstPersonHandler;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.scene.Spatial;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import com.radakan.graphics.mesh.anim.MeshAnimationController;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;

public class TestMeshLoading extends SimpleGame {

    public static void main(String[] args){
        TestMeshLoading app = new TestMeshLoading();
        app.setConfigShowMode(ConfigShowMode.AlwaysShow);
        app.start();
    }
    
    @Override
    protected void simpleInitGame() {
        OgreLoader loader = new OgreLoader();
        MaterialLoader matLoader = new MaterialLoader();
        
        try {
            SimpleResourceLocator srl = new SimpleResourceLocator(new URL("file:/D:/TestAnim/"));
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_MODEL, srl);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, srl);
            
            URL url2 = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, "xemna.material");
            URL url = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, "fingerman_50.mesh.xml");
            
            
//            matLoader.load(url2.openStream());
//            if (matLoader.getMaterials().size() > 0)
//                loader.setMaterials(matLoader.getMaterials());
            
            Spatial model = loader.loadModel(url);
            rootNode.attachChild(model);
            
            ((FirstPersonHandler)input).getKeyboardLookHandler().setMoveSpeed(250);
            
//            Quaternion q = new Quaternion();
//            q.fromAngles(/*-FastMath.HALF_PI*/0, FastMath.PI, 0);
//            rootNode.setLocalRotation(q);
            
            rootNode.updateGeometricState(0, true);
            rootNode.updateRenderState();
            
            MeshAnimationController animControl = (MeshAnimationController) model.getController(0);
            animControl.setAnimation("anim");
            //animControl.setSpeed(1f / 100f);
            
            
        } catch (IOException ex) {
            Logger.getLogger(TestMeshLoading.class.getName()).log(Level.SEVERE, null, ex);
        } catch (URISyntaxException ex) {
            Logger.getLogger(TestMeshLoading.class.getName()).log(Level.SEVERE, null, ex);
        }
        
    }

    
    
}
