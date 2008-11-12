package com.radakan.game.display;

import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.radakan.game.Game;
import com.radakan.game.config.GameConfigManager;
import com.radakan.game.debug.GameSysInfoManager;
import java.util.logging.Logger;

import com.gibbon.jme.context.*;

/**
 * GameDisplayManager maintains the main game GL context and makes it available
 * to other systems.
 * 
 * @author Kirill Vainer
 */
public class GameDisplayManager {

    private static final Logger logger = Logger.getLogger(GameDisplayManager.class.getName());
    
    private JmeContext context;
    
    /**
     * Depends on:
     *  - GameConfigManager
     *  - GameSysInfoManager (nodisplay)
     *  - GameDebugManager
     */
    public GameDisplayManager(){
        GameConfigManager config = Game.getConfigManager();
        final GameSysInfoManager sysInfo = Game.getSysInfoManager();
        
        try{
            context = JmeContext.create(LWJGLContext.class, JmeContext.CONTEXT_WINDOW);
            context.setSettings(config.getSettings());
            context.start();
            logger.fine("Display started");
            context.waitFor();
            logger.info("Display created successfuly");
        } catch (InterruptedException ex) {
            Game.getDebugManager().reportError("Interrupt while creating display", ex);
        } catch (InstantiationException ex){
            Game.getDebugManager().reportError("Display implementation cannot be loaded", ex);
        }
    }
    
    public JmeContext getContext(){
        return context;
    }
    
}
