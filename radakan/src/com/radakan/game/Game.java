package com.radakan.game;

import com.jme.scene.Node;
import com.jme.system.GameSettings;
import com.jme.system.PreferencesGameSettings;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import com.jmex.audio.AudioSystem;
import com.radakan.util.ErrorHandler;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.HashMap;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.prefs.Preferences;
import org.lwjgl.openal.AL10;
import org.lwjgl.opengl.ContextCapabilities;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL20;
import org.lwjgl.opengl.GLContext;

public class Game {

    private static final Logger logger = Logger.getLogger(Game.class.getName());
    
    private static HashMap<String, String> extToType = new HashMap<String, String>();
    private static GameSettings settings;
    private static boolean debugMode;
    
    public static ContextCapabilities caps;
    
    public static String vendor;
    public static String renderer;
    public static String apiVer;
    public static String glslVer;
    
    public static String alVendor;
    public static String alRenderer;
    public static String alVersion;
    
    /**
     * Initialize settings
     */
    static {
        settings = new PreferencesGameSettings(Preferences.userRoot().node("Radakan"));
        
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
        
        setDebug(false);
    }
    
    /**
     * Enable or disable debug mode.
     * 
     * During debug mode, more logging messages are displayed
     * and an in-game console is available.
     */
    public static void setDebug(boolean debug){
        debugMode = debug;
        
        if (debug){
            
            
            Logger.getLogger("").setLevel(Level.WARNING);
            Logger.getLogger("com.radakan").setLevel(Level.ALL);
            Logger.getLogger(ResourceLocatorTool.class.getName()).setLevel(Level.SEVERE);
            Logger.getLogger(Node.class.getName()).setLevel(Level.WARNING);
        }else{
            Logger.getLogger("").setLevel(Level.WARNING);
            Logger.getLogger("com.radakan").setLevel(Level.WARNING);
            Logger.getLogger(ResourceLocatorTool.class.getName()).setLevel(Level.SEVERE);
            Logger.getLogger(Node.class.getName()).setLevel(Level.WARNING);
        }
    }
    
    /**
     * Returns true if debug mode is set.
     */
    public static boolean isDebug(){
        return debugMode;
    }
    
    /**
     * Settings for the game. Cannot be null.
     */
    public static GameSettings getSettings(){
        return settings;
    }
    
    public static URL getResource(String resourceName){
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
     * Sets up resource locators for the given root.
     * The resources are organized into subdirectories.
     * 
     * See this forum thread for info on the directory layout:
     * http://radakan.org/forums/index.php/topic,759.msg5781.html#msg5781
     */
    public static void setupLocators(URL root){
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
        } catch (MalformedURLException ex){
            ErrorHandler.reportError("Syntax error in URL", ex);
        } catch (URISyntaxException ex){
            ErrorHandler.reportError("Syntax error in URI", ex);
        }
    }
    
    /**
     * Calls setupLocators for the default game root paths.
     * The default paths are game.pak and world.pak directories
     * inside the current working directory.
     */
    public static void setupDefaultLocators() throws IOException{
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
    
    /**
     * Queries the system for certain information about the audio and video card.
     */
    public static void querySystemInfo(){
        caps = GLContext.getCapabilities();

        if (AudioSystem.isCreated()){
            alVersion = AL10.alGetString(AL10.AL_VERSION);
            alRenderer = AL10.alGetString(AL10.AL_RENDERER);
            alVendor = AL10.alGetString(AL10.AL_VENDOR);
        }
        
        glslVer = GL11.glGetString(GL20.GL_SHADING_LANGUAGE_VERSION);
        apiVer = GL11.glGetString(GL11.GL_VERSION);
        renderer = GL11.glGetString(GL11.GL_RENDERER);
        vendor = GL11.glGetString(GL11.GL_VENDOR);
    }
    
    public static void exit(){
    }
    
    /**
     * Game version in number format
     */
    public static String getGameVersion(){
        return "0.50";
    }
    
    /**
     * Version prefix (prototype, alpha, beta, final..)
     */
    public static String getVersionPrefix(){
        return "Prototype";
    }
    
    public static String getGameName(){
        return "Radakan";
    }
    
}
