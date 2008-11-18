package com.radakan.game.gui;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.pass.RenderPass;
import com.jme.input.MouseInput;
import com.jme.renderer.Renderer;
import com.jme.scene.Node;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;
import com.jme.system.DisplaySystem;
import com.jme.system.GameSettings;
import com.radakan.game.Game;
import com.radakan.game.camera.GameCameraManager;
import com.radakan.game.input.FreeFlightHandler;
import com.radakan.game.input.InputType;
import com.radakan.game.world.GameWorldManager;
import java.net.URL;

/**
 * A task to load the actual in-game part of Radakan.
 * 
 * @see LoadScreen
 * 
 * @author Kirill Vainer
 */
public class GameLoadTask extends LoadingTask {

    private Node rootNode;
    private Renderer renderer;
    
    /**
     * Initializes the GameWorldManager.
     * 
     * @see GameWorldManager
     */
    @Override
    public void doInBackground() {
        try {
            GameSettings settings = Game.getConfigManager().getSettings();
            renderer = DisplaySystem.getDisplaySystem().getRenderer();
           
            GameCameraManager camMan = Game.createCameraManager();
            
            rootNode = new Node("Root Node");
            ZBufferState zbuf = renderer.createZBufferState();
            zbuf.setFunction(TestFunction.LessThan);
            rootNode.setRenderState(zbuf);
            
            settings.setBoolean("GameLightmaps", true);
            settings.setFloat("GameViewDistance", 64 * 3);
            
            URL worldURL = Game.getResourceManager().getResource("world.xml");
            GameWorldManager world = Game.createWorldManager();
            world.loadWorld(worldURL.openStream());

            rootNode.attachChild(world);

//        sPass = new DirectionalShadowMapPass(new Vector3f(0.1f,-1,0.1f));
//        pManager.add(sPass);
//
//        TileManager manager = (TileManager) ((Node)rootNode.getChild("World Node")).getChild("Tile Manager");
//        manager.setShadowManager(new ShadowMapPassManager(sPass));
            rootNode.updateGeometricState(0, true);
            rootNode.updateRenderState();
        } catch (Throwable ex) {
            Game.getDebugManager().reportError("Error occured while loading game", ex);
        }
    }

    /**
     * Adds a RenderPass to display the World, fades into the InGameScreen UIContext.
     * Initializes the input to dispatch events into the player controller.
     * 
     * @see GameWorldManager
     * @see InGameScreen
     */
    @Override
    public void done() {
        //MouseInput.get().setCursorVisible(false);
        Game.getInputManager().setInputListener(new FreeFlightHandler(renderer.getCamera()), InputType.CAMERA);
        //Camera cam = renderer.getCamera();
        //InputPass iPass = (InputPass) JmeContext.get().getPassManager().getPass("Input");
        //FirstPersonHandler handler = new FirstPersonHandler(cam, 10, 0.5f);
        //iPass.getInputHandler().addToAttachedHandlers(handler);
        
        RenderPass rPass = new RenderPass();
        rPass.add(rootNode);
        JmeContext.get().getPassManager().add(rPass);
        
        InGameScreen inGame = new InGameScreen();
        Game.getGUIManager().setContext(inGame, true);
    }

}
