package com.gibbon.radakan;

import com.gibbon.jme.context.ExitListenerPass;
import com.gibbon.jme.context.FengGuiPass;
import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.PassManager;
import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.gibbon.radakan.config.ConfigFrame;
import com.gibbon.radakan.error.ErrorReporter;
import com.gibbon.radakan.ui.UIManager;
import com.jme.image.Image.Format;
import com.jme.image.Texture;
import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MinificationFilter;
import com.jme.system.GameSettings;
import com.jme.system.PreferencesGameSettings;
import com.jme.util.TextureManager;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import java.io.File;
import java.net.URI;
import java.util.concurrent.Callable;
import java.util.prefs.Preferences;


public class Setup {

    public static void loadPasses(JmeContext cx){
        final PassManager pm = cx.getPassManager();
        pm.execute(new Callable<Object>(){
            public Object call(){
                pm.add(new ExitListenerPass(
                        new Callable<Object>(){
                            public Object call(){
                                JmeContext.get().dispose();
                                System.exit(0);
                                return null;
                            }
                        }));
                return null;
            }
        });
    }
    
    public static void loadGuiPass(JmeContext cx){
        FengGuiPass fg = new FengGuiPass(UIManager.getInstance());
        cx.getPassManager().add(fg);
        fg.waitFor();
    }
    
    public static JmeContext loadDisplay(){
        try {
            GameSettings gs = new PreferencesGameSettings(Preferences.userRoot().node("Radakan"));
            ConfigFrame config = new ConfigFrame(gs);
            config.setVisible(true);
            config.requestFocus();
            config.waitFor();

            // set custom settings here
            gs.set("title", SysInfo.getGameName() + " " + SysInfo.getVersionPrefix() + " " + SysInfo.getGameVersion());
            gs.setSamples(0);
            gs.setFramerate(60);
            gs.setVerticalSync(true);
            
            final JmeContext cx = JmeContext.create(LWJGLContext.class, JmeContext.CONTEXT_WINDOW);
            cx.setSettings(gs);
            cx.start();
            cx.waitFor();
            
            return cx;
        } catch (InstantiationException ex) {
            ErrorReporter.reportError("Failed to initialize Display implementor", ex);
        } catch (InterruptedException ex) {
            ErrorReporter.reportError("Recieved interrupt while waiting for Display", ex);
        } catch (Throwable ex){
            if (ex instanceof UnsatisfiedLinkError && ex.getMessage().equals("no lwjgl in java.library.path")){
                ErrorReporter.reportError("The native library \""+System.mapLibraryName("lwjgl")+"\" cannot be found", ex);
            }else{
                ErrorReporter.reportError("Error while creating 3D display", ex);
            }
        }
        
        return null;
    }
    
    public static void setupLocators(){
            String root = System.getProperty("user.dir");
            
            // setup audio locators
            URI musicDir = new File(root, "/data/music/").toURI();
            SimpleResourceLocator audio = new SimpleResourceLocator(musicDir);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_AUDIO, audio);

            // setup texture locators
            URI terDir = new File(root, "/data/tiles/").toURI();
            URI skyDir = new File(root, "/data/sky/").toURI();
            URI texDir = new File(root, "/data/textures/").toURI();

            SimpleResourceLocator ter = new SimpleResourceLocator(terDir);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, ter);
            SimpleResourceLocator tex = new SimpleResourceLocator(texDir);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, tex);
            SimpleResourceLocator sky = new SimpleResourceLocator(skyDir);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, sky);
            
            // setup shader locators
            URI shaderDir = new File(root, "/data/shaders/").toURI();
            SimpleResourceLocator shader = new SimpleResourceLocator(shaderDir);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_SHADER, shader);
            
            // setup model locators
            URI modelDir = new File(root, "/data/models/").toURI();
            SimpleResourceLocator model = new SimpleResourceLocator(modelDir);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_MODEL, model);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_MODEL, ter);
    }
    
    public static Texture loadTexture(String name, boolean flipY){
        return TextureManager.loadTexture(name,MinificationFilter.Trilinear,MagnificationFilter.Bilinear,1.0f,flipY);
    }
    
    /**
     * Loads an image but doesn't compress it
     * 
     * @param name
     * @param flipY
     * @return
     */
    public static Texture loadTextureUI(String name, boolean flipY){
        return TextureManager.loadTexture(name,MinificationFilter.BilinearNoMipMaps, MagnificationFilter.Bilinear,Format.GuessNoCompression,0.0f,flipY);
    }
    
    
}
