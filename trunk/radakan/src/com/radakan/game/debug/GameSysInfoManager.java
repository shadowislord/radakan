package com.radakan.game.debug;

import com.jme.util.geom.BufferUtils;
import com.jmex.audio.AudioSystem;

import java.nio.IntBuffer;
import java.util.Hashtable;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.Callable;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.lwjgl.LWJGLUtil;
import org.lwjgl.Sys;
import org.lwjgl.openal.AL10;
import org.lwjgl.opengl.ARBMultisample;
import org.lwjgl.opengl.ContextCapabilities;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL20;
import org.lwjgl.opengl.GLContext;

import com.gibbon.jme.context.*;

/**
 * GameSysInfoManager gathers system information including display and 
 * audio device information and makes it available to other systems.
 * GameDebugManager in particular makes use of this class to 
 * provide system information to allow remote debugging.
 * 
 * @author Kirill Vainer
 */
public class GameSysInfoManager {

	private static final Logger logger = Logger.getLogger(GameSysInfoManager.class.getName());
	
    private Hashtable<String, String> sysInfoParams = new Hashtable<String, String>();
    
    public  static final String GAME_NAME = "Radakan",
                                GAME_VERSION = "0.57",
                                GAME_SUFFIX = "Prototype";
    
    /**
     * Queries the system for certain information about the audio and video card.
     */
    public GameSysInfoManager(){
        sysInfoParams.put("game.name", GAME_NAME);
        sysInfoParams.put("game.version", GAME_VERSION);
        sysInfoParams.put("game.suffix", GAME_SUFFIX);
        sysInfoParams.put("game.fullname", GAME_NAME + " " + GAME_SUFFIX + " " + GAME_VERSION);
        
        sysInfoParams.put("lwjgl.version", ""+Sys.getVersion());
        sysInfoParams.put("lwjgl.debug", ""+LWJGLUtil.DEBUG);
        sysInfoParams.put("lwjgl.platform", ""+LWJGLUtil.getPlatform());
        
        sysInfoParams.put("cpu.processors", ""+Runtime.getRuntime().availableProcessors());
        
        sysInfoParams.put("os.locale", Locale.getDefault().toString());
        sysInfoParams.put("os.arch", System.getProperty("os.arch"));
        sysInfoParams.put("os.name", System.getProperty("os.name"));
        sysInfoParams.put("os.version", System.getProperty("os.version"));
        
        sysInfoParams.put("vm.name", System.getProperty("java.runtime.name"));
        sysInfoParams.put("vm.vendor", System.getProperty("java.vendor"));
        sysInfoParams.put("vm.version", System.getProperty("java.runtime.version"));
        sysInfoParams.put("vm.libs", System.getProperty("java.class.path")); 
    
        logger.fine("Basic system information gathered");
    }
    
    private void gatherDisplayInfoGL(){
        ContextCapabilities caps = GLContext.getCapabilities();
        
        sysInfoParams.put("graphics.glsl", GL11.glGetString(GL20.GL_SHADING_LANGUAGE_VERSION));
        sysInfoParams.put("graphics.version", GL11.glGetString(GL11.GL_VERSION));
        sysInfoParams.put("graphics.renderer", GL11.glGetString(GL11.GL_RENDERER));
        sysInfoParams.put("graphics.vendor", GL11.glGetString(GL11.GL_VENDOR));
        sysInfoParams.put("graphics.pixelshader", ""+caps.GL_ARB_fragment_program);
        sysInfoParams.put("graphics.vertexshader", ""+caps.GL_ARB_vertex_program);
        sysInfoParams.put("graphics.s3tc", ""+caps.GL_EXT_texture_compression_s3tc);
        sysInfoParams.put("graphics.floattexture", ""+caps.GL_ARB_texture_float);
        sysInfoParams.put("graphics.cubemaps", ""+caps.GL_ARB_texture_cube_map);
        
        logger.fine("Graphics Renderer: "+sysInfoParams.get("graphics.renderer"));
        logger.finer("Graphics Vendor: "+sysInfoParams.get("graphics.vendor"));
        logger.finer("Graphics Version: "+sysInfoParams.get("graphics.version"));
        
        if (caps.GL_ARB_multisample){
            IntBuffer ib = BufferUtils.createIntBuffer(16);
            GL11.glGetInteger(ARBMultisample.GL_SAMPLES_ARB, ib);
            sysInfoParams.put("graphics.multisample", ""+ib.get(0));
        }
        
        logger.finer("Multisample "+sysInfoParams.get("graphics.multisample"));
        
        if (AudioSystem.isCreated()){
            sysInfoParams.put("audio.version", AL10.alGetString(AL10.AL_VERSION)); 
            sysInfoParams.put("audio.renderer", AL10.alGetString(AL10.AL_RENDERER)); 
            sysInfoParams.put("audio.vendor", AL10.alGetString(AL10.AL_VENDOR)); 
            logger.fine("Audio Renderer: "+sysInfoParams.get("audio.renderer"));
        }
        
        logger.info("Display and audio device information gathered");
    }
    
    public void gatherDisplayInfo(){
        JmeContext.get().executeLater(new Callable<Object>(){
            public Object call(){
                gatherDisplayInfoGL();
                return null;
            }
        });
    }
    
    public Set<Map.Entry<String,String>> getEntries(){
        return sysInfoParams.entrySet();
    }
    
    public String getValue(String param){
        return sysInfoParams.get(param);
    }
    
}
