package com.radakan.gui.console;

import com.jme.renderer.ColorRGBA;
import com.jme.util.resource.ResourceLocatorTool;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.python.core.PyException;
import org.python.core.PyNone;
import org.python.core.PyObject;
import org.python.util.PythonInterpreter;

public class ScriptSystem implements ConsoleListener {

    private static final Logger logger = Logger.getLogger(ScriptSystem.class.getName());
    
    private String extension;
    
    // if using JYTHON
    private PythonInterpreter pythonEngine;
    
    private JmeConsole console;
    
    public ScriptSystem(JmeConsole console, boolean python){
        this.console = console;
        
        if (python){
            Properties props = new Properties();
            props.setProperty("python.home", "D:\\Programs\\Jython2.5a1");
            props.setProperty("python.path", "D:\\Programs\\Jython2.5a1\\Lib");

            PythonInterpreter.initialize(System.getProperties(), props, new String[]{ "" });
            
            extension = "py";
            
            pythonEngine = new PythonInterpreter();
            pythonEngine.setErr(System.err);
            pythonEngine.setOut(console.getOutputStream());
            
            pythonEngine.set("script", this);
            pythonEngine.set("logger", logger);
            pythonEngine.set("console", console);
            
            pythonEngine.exec("def loadLibrary(libraryName):\n" +
                              "    script.loadLibrary(libraryName)\n");
        }
        
        if (loadLibrary("main."+extension)){
            exec("main()\n");
        }
    }
    
    public boolean exec(String code){
        PyObject obj = null;
        try{
            obj = pythonEngine.eval(code);
            if (obj != null && !obj.toString().equals("None"))
                console.println(ColorRGBA.yellow, obj.toString());
        } catch (Throwable e){
            try{
                pythonEngine.exec(code);
            } catch (Throwable ex){
                console.println(ColorRGBA.red, ex.toString());
            }
        }

        return true;
    }
    
    public boolean loadLibrary(String libraryName){
        InputStream in;
        try{
            if (libraryName.startsWith("/")){
                File f = new File(libraryName);
                if (!f.exists()){
                    logger.log(Level.WARNING, "Cannot find library "+libraryName);
                    return false;
                }
                in = new FileInputStream(f);
            }else{
                URL url = ResourceLocatorTool.locateResource("Script", libraryName);
                if (url == null){
                    logger.log(Level.WARNING, "Cannot find library "+libraryName);
                    return false;
                }
                in = url.openStream();
            }

            if (pythonEngine != null){
                pythonEngine.execfile(in);
            }
            in.close();
            
            return true;
        } catch (IOException ex){
            logger.log(Level.WARNING, "IO Error on reading script library", ex);
        }
        
        return false;
    }
    
    public void commandTyped(JmeConsole console, String command) {
        exec(command);
    }

    

    
    
}
