package com.radakan.graphics.mesh.parser;

import com.jme.app.SimpleGame;
import com.jme.input.FirstPersonHandler;
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
        try {
            SimpleResourceLocator srl = new SimpleResourceLocator(new URL("file:/D:/TestAnim/"));
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_MODEL, srl);
            
            OgreLoader loader = new OgreLoader();
            Spatial model = loader.loadModel(new URL("file:/D:/TestAnim/fingerman_50.mesh.xml"));
            rootNode.attachChild(model);
            
            ((FirstPersonHandler)input).getKeyboardLookHandler().setMoveSpeed(250);
            
            rootNode.updateGeometricState(0, true);
            rootNode.updateRenderState();
            
            MeshAnimationController animControl = (MeshAnimationController) model.getController(0);
            animControl.setAnimation("anim");
        } catch (IOException ex) {
            Logger.getLogger(TestMeshLoading.class.getName()).log(Level.SEVERE, null, ex);
        } catch (URISyntaxException ex) {
            Logger.getLogger(TestMeshLoading.class.getName()).log(Level.SEVERE, null, ex);
        }
        
    }

    
    
}
