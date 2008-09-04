package com.radakan.graphics.mesh.parser;

import com.jme.app.SimpleGame;
import com.jme.input.FirstPersonHandler;
import com.jme.math.FastMath;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Node;
import com.jme.system.DisplaySystem;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import com.radakan.game.Game;
import com.radakan.graphics.mesh.anim.MeshAnimationController;
import java.io.IOException;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;

public class TestMeshLoading extends SimpleGame {

    private Node model;
    
    public static void main(String[] args){
        TestMeshLoading app = new TestMeshLoading();
        app.setConfigShowMode(ConfigShowMode.NeverShow);
        app.start();
    }
    
    protected void loadMeshModel(){
        OgreLoader loader = new OgreLoader();
        MaterialLoader matLoader = new MaterialLoader();
        
        try {
            Game.setupDefaultLocators();
            
            URL url2 = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, "Example.material");
            URL url = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, "ninja.mesh.xml");
            
            if (url2 != null){
                matLoader.load(url2.openStream());
                if (matLoader.getMaterials().size() > 0)
                    loader.setMaterials(matLoader.getMaterials());
            }
            
            model = (Node) loader.loadModel(url);
        } catch (IOException ex) {
            Logger.getLogger(TestMeshLoading.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    @Override
    protected void simpleInitGame() {
        Logger.getLogger("com.jme.scene.state.lwjgl").setLevel(Level.SEVERE);
        
        DisplaySystem.getDisplaySystem().setTitle("Test Mesh Instancing");
        display.getRenderer().setBackgroundColor(ColorRGBA.darkGray);
        ((FirstPersonHandler)input).getKeyboardLookHandler().setMoveSpeed(300);
        cam.setFrustumFar(20000f);
        loadMeshModel();
        //MeshCloner.setVBO(model);
        
        int ninjaN = 0;
        for (int x = -4; x < 5; x++){
            for (int y = -4; y < 5; y++){
                Node clone = MeshCloner.cloneMesh(model);
                
                clone.setLocalTranslation(150 * x,  0,  150 * y);
                rootNode.attachChild(clone);
                
                MeshAnimationController animControl = (MeshAnimationController) clone.getController(0);
                animControl.setAnimation("Walk");
                
                // rewind the animation a bit
                animControl.setTime(animControl.getAnimationLength("Walk") * FastMath.nextRandomFloat());
                
                //animControl.setFrameSkip(10);
                
                ninjaN++;
            }
        }
        
        System.out.println("WE HAVE "+ninjaN+" ninjas");
        
        rootNode.updateGeometricState(0, true);
        rootNode.updateRenderState();
    }

    
    
}
