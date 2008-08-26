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
    
    protected void setupLocators(URL root){
        // need to setup resource locators for paths in 
        // WORKING DIRECTORY, DATA PAK FILE, and LEVEL PAK FILE
        
        String urlString = root.toString();
        
        try{
            URL dataImages     = new URL(urlString + "/data/images/");
            URL dataModels     = new URL(urlString + "/data/models/");
            URL dataTiles      = new URL(urlString + "/data/tiles/");
            URL dataTilesMaps  = new URL(urlString + "/data/tiles/maps/");

            URL metaTextureset = new URL(urlString + "/meta/textureset/");
            URL codeScript     = new URL(urlString + "/code/script/");
            URL boot           = new URL(urlString + "/boot/");

            SimpleResourceLocator imagesLocator = new SimpleResourceLocator(dataImages);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, imagesLocator);
            
            SimpleResourceLocator modelsLocator = new SimpleResourceLocator(dataModels);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_MODEL, imagesLocator);
            
            SimpleResourceLocator tileLocator = new SimpleResourceLocator(dataTiles);
            ResourceLocatorTool.addResourceLocator("tile", tileLocator);
            
            SimpleResourceLocator tileMapLocator = new SimpleResourceLocator(dataTilesMaps);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, tileMapLocator);
            
            SimpleResourceLocator metaTSetLocator = new SimpleResourceLocator(metaTextureset);
            ResourceLocatorTool.addResourceLocator("textureset", metaTSetLocator);
            
            SimpleResourceLocator bootLocator = new SimpleResourceLocator(boot);
            ResourceLocatorTool.addResourceLocator("boot", bootLocator);
        } catch (MalformedURLException ex){
            
        } catch (URISyntaxException ex){
            
        }
    }
    
    protected void setupDefaultLocators() throws IOException{
        File gamePak = new File("game.pak");
        if (!gamePak.exists() || !gamePak.isDirectory())
            throw new FileNotFoundException(gamePak.toString());
        
        File worldPak = new File("world.pak");
        if (!worldPak.exists() || !worldPak.isDirectory())
            throw new FileNotFoundException(worldPak.toString());
        
        try {
            setupLocators(gamePak.toURI().toURL());
            setupLocators(worldPak.toURI().toURL());
        } catch (MalformedURLException ex) {
            ex.printStackTrace();
        }
    }
    
    protected void loadGame() throws IOException{
        setupDefaultLocators();
        
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
