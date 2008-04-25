package com.gibbon.radakan.error;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.radakan.Radakan;
import com.jmex.audio.AudioSystem;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.lang.Thread.UncaughtExceptionHandler;
import java.util.Locale;
import java.util.logging.Handler;
import java.util.logging.LogRecord;
import java.util.logging.Logger;
import org.lwjgl.LWJGLUtil;
import org.lwjgl.Sys;
import org.lwjgl.openal.AL10;
import org.lwjgl.opengl.ContextCapabilities;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL20;
import org.lwjgl.opengl.GLContext;

public class ErrorReporter {

    private static final StringBuffer log = new StringBuffer();
    
    public static class ErrorHandler extends Handler implements UncaughtExceptionHandler {

        @Override
        public void publish(LogRecord record) {
            log.append(record.toString());
            log.append('\n');
        }

        @Override
        public void flush() {
        }

        @Override
        public void close() throws SecurityException {
        }

        public void uncaughtException(Thread t, Throwable e) {
            ErrorReporter.reportError("Error occured while rendering", e);
        }
        
    }
    
    public static void reportError(String description, Throwable ex){
        ex.printStackTrace();
        
        ErrorFrame frame = new ErrorFrame(description, ex);
        frame.setVisible(true);
        frame.setLocationRelativeTo(null);
        frame.toFront();
        
        PrintStream stream = null;
        try{
            stream = new PrintStream("error.log");
        } catch (FileNotFoundException x){
        }
        
        stream.println("==Virtual Machine==");
        stream.println("Runtime: "+System.getProperty("java.runtime.name"));
        stream.println("Vendor: "+System.getProperty("java.vendor"));
        stream.println("Version: "+System.getProperty("java.runtime.version"));
        stream.println("Classpath: "+System.getProperty("java.class.path"));
        stream.println();

        stream.println("==System==");
        stream.println("OS Architecture: "+System.getProperty("os.arch"));
        stream.println("OS Name: "+System.getProperty("os.name"));
        stream.println("OS Version: "+System.getProperty("os.version"));
        stream.println("Locale: "+Locale.getDefault().toString());
        stream.println("Processor count: "+Runtime.getRuntime().availableProcessors());
        stream.println("Free memory: "+(Runtime.getRuntime().freeMemory()/1048576)+" MB");
        stream.println("Total memory: "+(Runtime.getRuntime().totalMemory()/1048576)+" MB");
        stream.println("Max memory: "+(Runtime.getRuntime().maxMemory()/1048576)+" MB");
        stream.println();

        if (JmeContext.get() != null && JmeContext.get().isActive()){
            stream.println("==Native bindings==");
            stream.println("Vendor: "+JmeContext.get().getName());
            stream.println("Version: "+Sys.getVersion());
            stream.println("Debug mode: "+LWJGLUtil.DEBUG);
            stream.println("Platform: "+LWJGLUtil.getPlatformName());
            stream.println();

            stream.println("==Display settings==");
            ContextCapabilities caps = Radakan.caps;
            stream.println("Adapter: "+Display.getAdapter());
            stream.println("Driver Version: "+Display.getVersion());
            stream.println("Vendor: "+Radakan.vendor);
            stream.println("Renderer: "+Radakan.renderer);
            stream.println("API Version: "+Radakan.apiVer);
            stream.println("GLSL Version: "+Radakan.glslVer);
            stream.println("Fragment Shaders: "+caps.GL_ARB_fragment_program);
            stream.println("Vertex Shaders: "+caps.GL_ARB_vertex_program);
            stream.println("Cubemaps: "+caps.GL_ARB_texture_cube_map);
            stream.println("Float textures: "+caps.GL_ARB_texture_float);
            stream.println("DOT3: "+caps.GL_ARB_texture_env_dot3);
            stream.println("Compression: "+caps.GL_EXT_texture_compression_s3tc);
            stream.println();

            if (AudioSystem.isCreated()){
                stream.println("==Audio settings==");
                stream.println("Vendor: "+Radakan.alVendor);
                stream.println("Renderer: "+Radakan.alRenderer);
                stream.println("Version: "+Radakan.alVersion);
                stream.println();
            }
            
            JmeContext.get().dispose();
        }

        stream.println("==Log==");
        stream.println(log.toString());
        stream.println();

        stream.println("==Exception==");
        stream.println("Description: "+description);
        ex.printStackTrace(stream);
        stream.println();
        stream.close();
    }
    
}
