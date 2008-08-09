/* Radakan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.radakan.util;

import com.gibbon.jme.context.JmeContext;
import com.jme.util.geom.BufferUtils;
import com.jmex.audio.AudioSystem;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.lang.Thread.UncaughtExceptionHandler;
import java.nio.IntBuffer;
import java.util.Locale;
import java.util.logging.ConsoleHandler;
import java.util.logging.Filter;
import java.util.logging.Level;
import java.util.logging.LogRecord;
import java.util.logging.Logger;
import org.lwjgl.LWJGLUtil;
import org.lwjgl.Sys;
import org.lwjgl.opengl.ARBMultisample;
import org.lwjgl.opengl.ContextCapabilities;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.GL11;

/**
 * @author Joshua Montgomery
 * @version 1.0.0
 * @created Aug 6, 2008
 */
public class ErrorHandler {
    
    //private static final StringBuffer log = new StringBuffer();
    private static boolean errorReported = false;
    
    public static class GameUncaughtExceptionHandler implements UncaughtExceptionHandler {

        public void uncaughtException(Thread t, Throwable e) {
            ErrorHandler.reportError("Error occured while rendering", e);
        }
        
    }
    
    public static void bindUncoughtExceptionHandler(){
        Thread.setDefaultUncaughtExceptionHandler(new GameUncaughtExceptionHandler());
        
        ConsoleHandler ch = new ConsoleHandler();
        ch.setLevel(Level.ALL);
        Logger.getLogger("com.radakan").addHandler(ch);
    }
    
    public static void writeSystemInfo(PrintStream stream){
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
            if (JmeContext.get().getName().equals("LWJGL")){
                stream.println("Version: "+Sys.getVersion());
                stream.println("Debug mode: "+LWJGLUtil.DEBUG);
                stream.println("Platform: "+LWJGLUtil.getPlatformName());
            }
            stream.println();

            stream.println("==Display settings==");
            if (JmeContext.get().getName().equals("LWJGL")){
                stream.println("Adapter: "+Display.getAdapter());
                stream.println("Driver Version: "+Display.getVersion());
            }
            stream.println("Vendor: "+SysInfo.vendor);
            stream.println("Renderer: "+SysInfo.renderer);
            stream.println("API Version: "+SysInfo.apiVer);
            stream.println("GLSL Version: "+SysInfo.glslVer);
            
            IntBuffer ib = BufferUtils.createIntBuffer(16);
            
            ContextCapabilities caps = SysInfo.caps;
            if (caps != null){
                stream.println("Fragment Shaders: "+caps.GL_ARB_fragment_program);
                stream.println("Vertex Shaders: "+caps.GL_ARB_vertex_program);
                stream.println("Cubemaps: "+caps.GL_ARB_texture_cube_map);
                stream.println("Float textures: "+caps.GL_ARB_texture_float);
                stream.println("DOT3: "+caps.GL_ARB_texture_env_dot3);
                stream.println("Compression: "+caps.GL_EXT_texture_compression_s3tc);
                if (caps.GL_ARB_multisample){
                    GL11.glGetInteger(ARBMultisample.GL_SAMPLE_BUFFERS_ARB, ib);
                    stream.println("Antialiasing/Multisampling: "+ib.get(0));
                }
            }
            stream.println();

            if (AudioSystem.isCreated()){
                stream.println("==Audio settings==");
                stream.println("Vendor: "+SysInfo.alVendor);
                stream.println("Renderer: "+SysInfo.alRenderer);
                stream.println("Version: "+SysInfo.alVersion);
                stream.println();
            }
            
            JmeContext.get().dispose();
        }
    }
    
    public static void reportError(String description, Throwable ex){
        ex.printStackTrace();
        
        // XXX: Okay to drop error?
        if (errorReported)
            return;
        else
            errorReported = true;
        
        ErrorFrame frame = new ErrorFrame(description, ex, null);
        frame.setVisible(true);
        frame.setLocationRelativeTo(null);
        frame.toFront();
        
        PrintStream stream = null;
        try{
            stream = new PrintStream("error.log");
        } catch (FileNotFoundException x){
        }
        
        writeSystemInfo(stream);

        //stream.println("==Log==");
        //stream.println(log.toString());
        //stream.println();

        stream.println("==Exception==");
        stream.println("Description: "+description);
        ex.printStackTrace(stream);
        stream.println();
        stream.close();
    }
    
}
