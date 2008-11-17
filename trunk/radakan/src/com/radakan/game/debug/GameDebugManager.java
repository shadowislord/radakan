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
package com.radakan.game.debug;

import com.gibbon.jme.context.JmeContext;
import com.jme.scene.Node;
import com.jme.system.DisplaySystem;
import com.jme.system.lwjgl.LWJGLDisplaySystem;
import com.jme.util.geom.BufferUtils;
import com.jmex.audio.AudioSystem;
import com.radakan.game.Game;

import com.radakan.game.debug.GameSysInfoManager;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.lang.Thread.UncaughtExceptionHandler;
import java.nio.IntBuffer;
import java.util.Locale;
import java.util.Map;
import java.util.logging.ConsoleHandler;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.lwjgl.LWJGLUtil;
import org.lwjgl.Sys;
import org.lwjgl.opengl.ARBMultisample;
import org.lwjgl.opengl.ContextCapabilities;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.GL11;

/**
 * GameDebugManager recieves reports of errors from game systems and displays
 * an error message to the user, it also listens for uncought exceptions and 
 * makes sure to display them as well.
 * 
 * @author Joshua Montgomery
 * @author Kirill Vainer
 * @version 1.1.0
 * @created Aug 6, 2008
 */
public class GameDebugManager {
    
	private static final Logger logger = Logger.getLogger(GameDebugManager.class.getName());
	
    //private static final StringBuffer log = new StringBuffer();
    private boolean errorReported = false;
    private boolean debugMode;
    
    public GameDebugManager(boolean debug){
    	Logger global = Logger.getLogger("");
    	Handler[] handlers = global.getHandlers();
        for (Handler h : handlers){
        	global.removeHandler(h);
        }
    	
        ConsoleHandler ch = new ConsoleHandler();
        ch.setLevel(Level.ALL);
        Logger.getLogger("").addHandler(ch);
        
        setDebug(debug);
        loadExceptionHandler();
    }
    
    public void loadExceptionHandler(){
        Thread.setDefaultUncaughtExceptionHandler(new GameUncaughtExceptionHandler());
        logger.finer("Uncought exception handler set for thread '"+Thread.currentThread().getName()+"'");
    }
    
    public class GameUncaughtExceptionHandler implements UncaughtExceptionHandler {
        @Override
        public void uncaughtException(Thread t, Throwable e) {
            reportError("An unexpected error has occured", e);
        }
    }
    
    /**
     * Enable or disable debug mode.
     * 
     * During debug mode, more logging messages are displayed
     * and an in-game console is available.
     */
    public void setDebug(boolean debug){
        debugMode = debug;
        
        if (debug){
            Logger.getLogger("").setLevel(Level.WARNING);
            Logger.getLogger("com.radakan").setLevel(Level.ALL);
            Logger.getLogger(Node.class.getName()).setLevel(Level.WARNING);
            logger.finer("Debug mode is ON");
        }else{
            Logger.getLogger("").setLevel(Level.WARNING);
            Logger.getLogger("com.radakan").setLevel(Level.WARNING);
            Logger.getLogger(Node.class.getName()).setLevel(Level.WARNING);
            logger.finer("Debug mode is OFF");
        }
    }
    
    /**
     * Returns true if debug mode is set.
     */
    public boolean isDebug(){
        return debugMode;
    }
    
    public void writeSystemInfo(PrintStream stream){
        GameSysInfoManager sysInfo = Game.getSysInfoManager();
        
        for (Map.Entry<String,String> ent : sysInfo.getEntries()){
            stream.println(ent.getKey()+": "+ent.getValue());
        }
    }
    
    public void reportError(String description, Throwable ex){
        if (ex != null)
            ex.printStackTrace();
        
        if (errorReported){
        	logger.warning("Error dropped: "+ex.getMessage());
            return;
        }else{
            errorReported = true;
        }
        
        ErrorFrame frame = new ErrorFrame(description, ex, null);
        frame.setVisible(true);
        frame.setLocationRelativeTo(null);
        frame.toFront();
        
        try{
            PrintStream stream = new PrintStream("error.log");
            
            writeSystemInfo(stream);
            stream.println("==Exception==");
            stream.println("Description: "+description);
            ex.printStackTrace(stream);
            stream.println();
            stream.close();
        } catch (FileNotFoundException x){
        }

        if (JmeContext.get() != null){
            JmeContext.get().dispose();
        }else{
            DisplaySystem.getDisplaySystem().close();
        }
    }
    
}
