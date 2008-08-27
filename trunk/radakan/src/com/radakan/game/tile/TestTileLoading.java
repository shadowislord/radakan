package com.radakan.game.tile;

import com.jme.app.SimpleGame;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import com.gibbon.jme.context.*;
import com.jme.scene.Node;
import com.jme.util.GameTaskQueueManager;
import com.radakan.game.Game;
import com.radakan.game.world.World;
import com.radakan.util.ErrorHandler;
import java.io.FileNotFoundException;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.util.concurrent.Callable;
import java.util.logging.Level;
import java.util.logging.Logger;

public class TestTileLoading extends SimpleGame {

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
        
        settings.setBoolean("GameLightmaps", true);
        settings.setFloat("GameViewDistance", 64*3);
        
        URL worldURL = ResourceLocatorTool.locateResource("boot", "world.xml");
        World world = new World(display.getRenderer(), settings);
        world.loadWorld(worldURL.openStream());
        
        rootNode.attachChild(world);
        
        rootNode.updateGeometricState(0, true);
        rootNode.updateRenderState();
    }
    
    @Override
    protected void simpleInitGame() {
        try{
            loadGame();

//            GameTaskQueueManager.getManager().render(new Callable<Object>(){
//                public Object call(){
//                    rootNode.updateRenderState();
//                    rootNode.updateGeometricState(0, true);
//                    return null;
//                }
//            });
        } catch (Throwable t){
            t.printStackTrace();
        }
    }
    
}
