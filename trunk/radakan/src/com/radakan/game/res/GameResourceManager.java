package com.radakan.game.res;

import com.jme.scene.Spatial;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import com.radakan.game.Game;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.HashMap;
import java.util.logging.Logger;

public class GameResourceManager {

    private static final Logger logger = Logger.getLogger(GameResourceManager.class.getName());
    
    private HashMap<String, String> extToType = new HashMap<String, String>();
    private String tilePath;
    
    /**
     * Calls setupLocators for the default game root paths.
     * The default paths are game.pak and world.pak directories
     * inside the current working directory.
     */
    public GameResourceManager() {
        extToType.put("dds", ResourceLocatorTool.TYPE_TEXTURE);
        extToType.put("png", ResourceLocatorTool.TYPE_TEXTURE);
        extToType.put("bmp", ResourceLocatorTool.TYPE_TEXTURE);
        extToType.put("gif", ResourceLocatorTool.TYPE_TEXTURE);
        extToType.put("jpg", ResourceLocatorTool.TYPE_TEXTURE);
        extToType.put("tga", ResourceLocatorTool.TYPE_TEXTURE);
        
        extToType.put("jme",      ResourceLocatorTool.TYPE_MODEL);
        extToType.put("3ds",      ResourceLocatorTool.TYPE_MODEL);
        extToType.put("md2",      ResourceLocatorTool.TYPE_MODEL);
        extToType.put("md3",      ResourceLocatorTool.TYPE_MODEL);
        extToType.put("mesh.xml", ResourceLocatorTool.TYPE_MODEL);
        
        extToType.put("scene", "scene");
        
        String userDir = "../common";
        try {
            File gamePak = new File(userDir, "game.pak");
            if (!gamePak.exists() || !gamePak.isDirectory())
                throw new FileNotFoundException(gamePak.toString());

            File worldPak = new File(userDir, "world.pak");
            if (!worldPak.exists() || !worldPak.isDirectory())
                throw new FileNotFoundException(worldPak.toString());
        
            setupLocators(gamePak.toURI().toURL());
            setupLocators(worldPak.toURI().toURL());
            tilePath = worldPak.toURI().normalize().toURL() + "data/tiles/";
            
            logger.finer("Tile path is "+tilePath);
            logger.fine("Resource paths setup successfuly");
        } catch (MalformedURLException ex) {
            Game.getDebugManager().reportError("Malformed URL", ex);
        } catch (FileNotFoundException ex){
            Game.getDebugManager().reportError("Failed to setup resource paths", ex);
        }
    }
    
    public String getTilePath(){
        return tilePath;
    }
    
    public URL getResource(String resourceName){
        resourceName = resourceName.trim().toLowerCase();
        
        if (resourceName.endsWith("world.xml"))
            return ResourceLocatorTool.locateResource("boot", resourceName);
        
        if (resourceName.startsWith("tile") && resourceName.endsWith(".xml"))
            return ResourceLocatorTool.locateResource("tile", resourceName);
        
        if (resourceName.endsWith("entities.xml"))
            return ResourceLocatorTool.locateResource("tile", resourceName);
        
        int index = resourceName.indexOf(".");
        if (index < 1){
            logger.warning("Cannot determine extension of resource "+resourceName);
            return null;
        }
        
        String ext = resourceName.substring(index+1);
        String type = extToType.get(ext);
        
        if (type == null)
            return null;
        
        return ResourceLocatorTool.locateResource(type, resourceName);
    }
    
    /**
     * Loads a model at the given path. All jME's model formats are supported, 
     * plus Mesh.XML and MD5 model formats. 
     * 
     * @param path The name of the model, e.g "mymodel.mesh.xml".
     * @return A scene graph object representing the model.
     * @throws java.io.IOException If the model cannot be found or the foramt cannot be determined.
     */
    public Spatial loadModel(String path) throws IOException {
        if (path.startsWith("/models/"))
            path = path.substring(8);
        
        URL url = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, path);
        if (url == null)
            throw new IOException("Cannot locate model "+path);
        
        // path is local to the file system
        int index = path.indexOf(".");
        if (index == -1)
            throw new IOException("No extension available for model file");
        
        String ext = path.substring(index+1);
        return ModelLoader.loadModel(url, ext);
    }
    
    /**
     * Sets up resource locators for the given root.
     * The resources are organized into subdirectories.
     * 
     * See this forum thread for info on the directory layout:
     * http://radakan.org/forums/index.php/topic,759.msg5781.html#msg5781
     */
    public void setupLocators(URL root){
        // need to setup resource locators for paths in 
        // WORKING DIRECTORY, DATA PAK FILE, and LEVEL PAK FILE
        
        String urlString = root.toString();
        if (urlString.endsWith("/"))
            urlString = urlString.substring(0, urlString.length() -1);
        
        try{
            URL dataImages     = new URL(urlString + "/data/images/");
            URL dataModels     = new URL(urlString + "/data/models/");
            URL dataTiles      = new URL(urlString + "/data/tiles/");
            URL dataScenes     = new URL(urlString + "/data/scenes/");
            URL dataTilesMaps  = new URL(urlString + "/data/tiles/maps/");

            URL metaEntity     = new URL(urlString + "/meta/entity/");
            URL metaTextureset = new URL(urlString + "/meta/textureset/");
            URL codeScript     = new URL(urlString + "/code/script/");
            URL boot           = new URL(urlString + "/boot/");

            SimpleResourceLocator imagesLocator = new SimpleResourceLocator(dataImages);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, imagesLocator);
            
            SimpleResourceLocator modelsLocator = new SimpleResourceLocator(dataModels);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_MODEL, modelsLocator);
            
            SimpleResourceLocator tileLocator = new SimpleResourceLocator(dataTiles);
            ResourceLocatorTool.addResourceLocator("tile", tileLocator);
            
            SimpleResourceLocator sceneLocator = new SimpleResourceLocator(dataScenes);
            ResourceLocatorTool.addResourceLocator("scene", sceneLocator);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_MODEL, sceneLocator);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, sceneLocator);
            
            SimpleResourceLocator tileMapLocator = new SimpleResourceLocator(dataTilesMaps);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, tileMapLocator);
            
            SimpleResourceLocator metaTSetLocator = new SimpleResourceLocator(metaTextureset);
            ResourceLocatorTool.addResourceLocator("textureset", metaTSetLocator);
            
            SimpleResourceLocator metaEntityLocator = new SimpleResourceLocator(metaEntity);
            ResourceLocatorTool.addResourceLocator("entity", metaEntityLocator);
            
            SimpleResourceLocator bootLocator = new SimpleResourceLocator(boot);
            ResourceLocatorTool.addResourceLocator("boot", bootLocator);
            
            SimpleResourceLocator codeLocator = new SimpleResourceLocator(codeScript);
            ResourceLocatorTool.addResourceLocator("Script", codeLocator);
        
            logger.finer("Set resource paths for "+root.toURI().normalize());
        } catch (MalformedURLException ex){
            Game.getDebugManager().reportError("Syntax error in URL", ex);
        } catch (URISyntaxException ex){
            Game.getDebugManager().reportError("Syntax error in URI", ex);
        }
    }
    
}
