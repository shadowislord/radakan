package com.radakan.game.tile;

import com.jme.app.SimpleGame;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import java.io.File;
import java.net.URL;
import com.gibbon.jme.context.*;
import com.jme.scene.Node;
import com.jme.util.GameTaskQueueManager;
import com.radakan.util.ErrorHandler;
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

    @Override
    protected void simpleInitGame() {
        try{
            SimpleResourceLocator srl = new SimpleResourceLocator(new File("C:\\Users\\Kirill\\Desktop\\TILE_EXPORT\\").toURI());
            ResourceLocatorTool.addResourceLocator("tile", srl);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, srl);
            
            srl = new SimpleResourceLocator(TestTileLoading.class.getResource("/com/radakan/data/texturesets/images/"));
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, srl);

            URL worldMetaURL = ResourceLocatorTool.locateResource("tile", "world.xml");
            
            TileLoader.readWorldMeta(worldMetaURL);
            TileLoader.readTextureSets(new File("E:\\Radakan\\src\\com\\radakan\\data\\texturesets\\"));
            TileLoader.readEntityTypes(new File("E:\\Radakan\\src\\com\\radakan\\data\\entities\\"));
            
            TileManager manager = new TileManager(display.getRenderer());
            rootNode.attachChild(manager);

            GameTaskQueueManager.getManager().render(new Callable<Object>(){
                public Object call(){
                    rootNode.updateRenderState();
                    rootNode.updateGeometricState(0, true);
                    return null;
                }
            });
            
        } catch (Throwable t){
            t.printStackTrace();
        }
    }
    
}
