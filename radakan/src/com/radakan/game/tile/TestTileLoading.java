package com.radakan.game.tile;

import com.jme.app.SimpleGame;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import com.gibbon.jme.context.*;
import com.jme.app.SimplePassGame;
import com.jme.input.FirstPersonHandler;
import com.jme.input.MouseInput;
import com.jme.math.Vector3f;
import com.jme.renderer.pass.RenderPass;
import com.jme.scene.Node;
import com.jme.util.GameTaskQueueManager;
import com.radakan.game.Game;
import com.radakan.game.util.ShadowMapPassManager;
import com.radakan.game.world.World;
import com.radakan.jme.hdrreader.DirectionalShadowMapPass;
import com.radakan.util.ErrorHandler;
import java.io.FileNotFoundException;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.util.concurrent.Callable;
import java.util.logging.Level;
import java.util.logging.Logger;

public class TestTileLoading extends SimplePassGame {

    private static DirectionalShadowMapPass sPass;
    
    public static void main(String[] args){
        ErrorHandler.bindUncaughtExceptionHandler();
        
        Logger.getLogger("com.radakan").setLevel(Level.ALL);
        Logger.getLogger(ResourceLocatorTool.class.getName()).setLevel(Level.SEVERE);
        Logger.getLogger(Node.class.getName()).setLevel(Level.WARNING);
        
        TestTileLoading app = new TestTileLoading();
        app.setConfigShowMode(ConfigShowMode.AlwaysShow);
        app.start();
    }
    
    protected void loadGame() throws IOException{
        Game.setupDefaultLocators();
        Game.querySystemInfo();
        
        ((FirstPersonHandler)input).getKeyboardLookHandler().setMoveSpeed(10f);
        ((FirstPersonHandler)input).getMouseLookHandler().getMouseLook().setSpeed(0.3f);
        
        settings.setBoolean("GameLightmaps", true);
        settings.setFloat("GameViewDistance", 64*3);
        
        URL worldURL = ResourceLocatorTool.locateResource("boot", "world.xml");
        World world = new World(display.getRenderer(), settings);
        world.loadWorld(worldURL.openStream());
        
        rootNode.attachChild(world);
        
        RenderPass rPass = new RenderPass();
        rPass.add(statNode);
        rPass.add(rootNode);
        pManager.add(rPass);

//        sPass = new DirectionalShadowMapPass(new Vector3f(0.1f,-1,0.1f));
//        pManager.add(sPass);
//
//        TileManager manager = (TileManager) ((Node)rootNode.getChild("World Node")).getChild("Tile Manager");
//        manager.setShadowManager(new ShadowMapPassManager(sPass));
        
        rootNode.updateGeometricState(0, true);
        rootNode.updateRenderState();
    }
    
    @Override
    protected void simpleInitGame() {
        try{
            loadGame();
        } catch (Throwable t){
            t.printStackTrace();
        }
    }
    
}
