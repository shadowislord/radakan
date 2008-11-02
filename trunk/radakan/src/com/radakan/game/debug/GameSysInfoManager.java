package com.radakan.game.debug;

import com.jme.util.geom.BufferUtils;
import com.jmex.audio.AudioSystem;

import java.nio.IntBuffer;
import java.util.Hashtable;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.Callable;

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

public class GameSysInfoManager {

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
    }
    
    private void gatherDisplayInfoGL(){
        ContextCapabilities caps = GLContext.getCapabilities();
        
        sysInfoParams.put("display.adapter", Display.getAdapter());
        sysInfoParams.put("display.version", Display.getVersion());
        
        sysInfoParams.put("graphics.glsl", GL11.glGetString(GL20.GL_SHADING_LANGUAGE_VERSION));
        sysInfoParams.put("graphics.version", GL11.glGetString(GL11.GL_VERSION));
        sysInfoParams.put("graphics.renderer", GL11.glGetString(GL11.GL_RENDERER));
        sysInfoParams.put("graphics.vendor", GL11.glGetString(GL11.GL_VENDOR));
        sysInfoParams.put("graphics.pixelshader", ""+caps.GL_ARB_fragment_program);
        sysInfoParams.put("graphics.vertexshader", ""+caps.GL_ARB_vertex_program);
        sysInfoParams.put("graphics.s3tc", ""+caps.GL_EXT_texture_compression_s3tc);
        sysInfoParams.put("graphics.floattexture", ""+caps.GL_ARB_texture_float);
        sysInfoParams.put("graphics.cubemaps", ""+caps.GL_ARB_texture_cube_map);
        
        if (caps.GL_ARB_multisample){
            IntBuffer ib = BufferUtils.createIntBuffer(16);
            GL11.glGetInteger(ARBMultisample.GL_SAMPLE_BUFFERS_ARB, ib);
            sysInfoParams.put("graphics.multisample", ""+ib.get(0));
        }
        
        if (AudioSystem.isCreated()){
            sysInfoParams.put("audio.version", AL10.alGetString(AL10.AL_VERSION)); 
            sysInfoParams.put("audio.renderer", AL10.alGetString(AL10.AL_RENDERER)); 
            sysInfoParams.put("audio.vendor", AL10.alGetString(AL10.AL_VENDOR)); 
        }
    }
    
    public void gatherDisplayInfo(){
        JmeContext.get().executeLater(new Callable<Object>(){
            public Object call(){
                gatherDisplayInfo();
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
