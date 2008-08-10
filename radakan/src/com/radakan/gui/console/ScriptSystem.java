package com.radakan.gui.console;

import com.jme.renderer.ColorRGBA;
import com.jme.util.resource.ResourceLocatorTool;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.script.Bindings;
import javax.script.ScriptContext;
import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;
import javax.script.ScriptException;

public class ScriptSystem implements ConsoleListener {

    private static final Logger logger = Logger.getLogger(ScriptSystem.class.getName());
    
    private ScriptEngineManager engineManager;
    private ScriptEngine engine;
    private Bindings bindings;
    private JmeConsole console;
    
    public ScriptSystem(JmeConsole console){
        this.console = console;
        
        
        engineManager = new ScriptEngineManager();
        engine = engineManager.getEngineByName("js");
        bindings = engine.createBindings();
        engine.setBindings(bindings, ScriptContext.ENGINE_SCOPE);

        bindings.put("script", this);
        bindings.put("logger", logger);
        bindings.put("console", console);
        
        try {
            engine.eval("function loadLibrary(libraryName){\n" +
                        "    script.loadLibrary(libraryName);\n" + 
                        "}\n");
        } catch (ScriptException ex) {
            logger.log(Level.WARNING, "Syntax error init", ex);
        }
        
        if (loadLibrary("main.js")){
            exec("main();");
        }
    }
    
    public boolean exec(String code){
        try {
            Object obj = engine.eval(code);
            if (obj != null)
                console.println(ColorRGBA.yellow, obj.toString());
            
            return true;
        } catch (ScriptException ex) {
            String msg = ex.getMessage();
            msg = msg.substring(msg.indexOf(':'));
            console.println(ColorRGBA.red, msg);
        }
        
        return false;
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

            InputStreamReader reader = new InputStreamReader(in);
            engine.eval(reader);
            reader.close();
            
            return true;
        } catch (ScriptException ex) {
            logger.log(Level.WARNING, "Syntax error in script library", ex);
        } catch (IOException ex){
            logger.log(Level.WARNING, "IO Error on reading script library", ex);
        }
        
        return false;
    }
    
    public void commandTyped(JmeConsole console, String command) {
        exec(command);
    }

    

    
    
}
