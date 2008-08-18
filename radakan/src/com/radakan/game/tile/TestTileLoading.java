package com.radakan.game.tile;

import com.jme.app.SimpleGame;
import com.jme.scene.Spatial;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import java.io.File;
import java.net.URL;

public class TestTileLoading extends SimpleGame {

    public static void main(String[] args){
        TestTileLoading app = new TestTileLoading();
        app.setConfigShowMode(ConfigShowMode.AlwaysShow);
        app.start();
    }

    @Override
    protected void simpleInitGame() {
        SimpleResourceLocator srl = new SimpleResourceLocator(new File("C:\\Users\\Kirill\\Desktop\\TILE_EXPORT\\").toURI());
        ResourceLocatorTool.addResourceLocator("tile", srl);
        
        URL worldMetaURL = ResourceLocatorTool.locateResource("tile", "world.xml");
        
        TileLoader loader = new TileLoader(false);
        loader.readWorldMeta(worldMetaURL);
        Spatial tile = loader.loadTile(0, 0);
        
        rootNode.attachChild(tile);
        
        rootNode.updateRenderState();
        rootNode.updateGeometricState(0, true);
    }
    
}
