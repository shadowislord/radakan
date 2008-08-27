package com.radakan.game;

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
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.prefs.Preferences;
import org.lwjgl.openal.AL10;
import org.lwjgl.opengl.ContextCapabilities;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL20;
import org.lwjgl.opengl.GLContext;

public class Game {

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
            Logger.getLogger("").setLevel(Level.FINEST);
        }else{
            Logger.getLogger("").setLevel(Level.WARNING);
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
    
    public static void setupLocators(URL root){
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
            ErrorHandler.reportError("Syntax error in URL", ex);
        } catch (URISyntaxException ex){
            ErrorHandler.reportError("Syntax error in URI", ex);
        }
    }
    
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
        return "0.40";
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
