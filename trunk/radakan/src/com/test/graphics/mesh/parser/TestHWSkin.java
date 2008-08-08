package com.test.graphics.mesh.parser;

import com.gibbon.jme.context.JmeContext;


public class TestHWSkin {

    public static void initGame(JmeContext cx){
        
    }
    
    public static void main(String[] args){
        //Logger.getLogger("").setLevel(Level.WARNING);
        
//        GameSettings settings
//                = new PreferencesGameSettings(Preferences.userRoot().node("test-jmecontext"));
//        
//        settings.set("title", "Test Main Menu");
//        
//        JmeContext context = null;
//        try{
//            ConfigFrame g = new ConfigFrame(settings);
//            g.setVisible(true);
//            g.waitFor();
//
////            settings.setWidth(720);
////            settings.setHeight(480);
//            
//            context = JmeContext.create(LWJGLContext.class, JmeContext.CONTEXT_WINDOW);
//            context.setSettings(settings);
//            context.start();
//            context.waitFor();
//        } catch (Exception ex){
//            ErrorReporter.reportError("Error while initializing display", ex);
//        }
//        
//        final JmeContext cx = context;
//        
//        // add input pass
//        InputPass input = new InputPass(null, true);
//        cx.getPassManager().add(input);
//        
//        // add exit listener
//        ExitListenerPass elp = new ExitListenerPass(new Callable<Object>(){
//            public Object call() throws Exception {
//                cx.dispose();
//                System.exit(0);
//                return null;
//            }
//        });
//        cx.getPassManager().add(elp);
//        
//        initGame(cx);
    }
    
}
