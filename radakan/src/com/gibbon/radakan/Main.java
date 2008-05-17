/*
 * Radakan is free software: you can redistribute it and/or modify
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

package com.gibbon.radakan;

import com.gibbon.jme.context.InputPass;
import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.PassType;
import com.gibbon.jme.context.RenderPass;
import com.gibbon.jme.context.SoundPass;
import com.gibbon.meshparser.SceneLoader;
import com.gibbon.radakan.entity.*;
import com.gibbon.radakan.error.ErrorReporter;
import com.gibbon.radakan.menu.LoadingController;
import com.gibbon.radakan.player.PlayerController;

import com.jme.bounding.BoundingBox;
import com.jme.bounding.CollisionTree.Type;
import com.jme.bounding.CollisionTreeManager;

import com.jme.image.Texture;

import com.jme.light.PointLight;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import com.jme.renderer.Renderer;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import com.jme.scene.VBOInfo;
import com.jme.scene.shape.Quad;
import com.jme.scene.state.BlendState;
import com.jme.scene.state.BlendState.DestinationFunction;
import com.jme.scene.state.BlendState.SourceFunction;
import com.jme.scene.state.CullState;
import com.jme.scene.state.LightState;
import com.jme.scene.state.TextureState;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;
import com.jme.util.resource.ResourceLocatorTool;
import com.jmex.audio.AudioSystem;
import com.jmex.audio.AudioTrack;
import com.jmex.audio.MusicTrackQueue;
import com.jmex.audio.MusicTrackQueue.RepeatType;
import com.model.md5.ModelNode;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.concurrent.Callable;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Main {

    private static Node walkableNode = new Node("walkable");
    private static Node rootNode     = new Node("root");
    
    private static RenderPass    renderPass;
    private static RenderPass    skyPass;
    private static InputPass     inputPass;
    
    private static LoadingController loading;

    public static void setVbo(Spatial spatial){
        spatial.setIsCollidable(true);
        
        if (spatial instanceof Node){
            Node n = (Node)spatial;
            if (n.getChildren() == null)
                return;
            
            for (Spatial s: n.getChildren()){
                setVbo(s);
            }
        }else if (spatial instanceof TriMesh){
            TriMesh mesh = (TriMesh) spatial;
            mesh.setVBOInfo(new VBOInfo(true));
            //if (mesh.getName().equalsIgnoreCase("terrain")){
                System.out.println("MESH: "+mesh+" is COLLIDABLE");
                mesh.setIsCollidable(true);
                CollisionTreeManager.getInstance().generateCollisionTree(Type.AABB, mesh, true);
            //}
        }
    }
    
    public static Spatial loadTerrain(JmeContext cx) throws IOException{
        CollisionTreeManager.getInstance().setTreeType(Type.AABB);
        
//        URL bootstrap = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, "bootstrap.jme");
//        if (bootstrap == null)
//            throw new IllegalStateException("Cannot find world file");
//        
//        final Spatial world = (Spatial) BinaryImporter.getInstance().load(bootstrap);
        
        final Node world = new Node("WORLD");
        
        SceneLoader loader = new SceneLoader();
        
        URL url = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, "area.scene");
        InputStream in = url.openStream();
        loader.load(in);
        in.close();
        
        world.attachChild(loader.getScene());
        
        world.setModelBound(new BoundingBox());
        world.updateModelBound();
        
        if (cx.getRenderer().supportsVBO()){
            cx.execute(new Callable<Void>(){
                public Void call(){
                    setVbo(world);
                    return null;
                }
            });
        }
        
//        world.lockBounds();
//        world.lockBranch();
//        world.lockShadows();
//        world.lockTransforms();
        
        return world;
    }
    
    public static Spatial loadLoadingScreen(JmeContext cx){
        Renderer r = cx.getRenderer();
        
        float width = r.getWidth();
        float height = r.getHeight();
        
        // XXX: WTF? Why do I have to multiply height by aspect
        //      to get correct results.. so random
        float aspect = height / width;
        
        BlendState as = r.createBlendState();
        as.setBlendEnabled(true);
        as.setSourceFunction(SourceFunction.SourceAlpha);
        as.setDestinationFunction(DestinationFunction.OneMinusSourceAlpha);

        Quad background = new Quad("Background", width, height * aspect );
        background.setRenderQueueMode(Renderer.QUEUE_ORTHO);
        background.setColorBuffer(null);
        background.setDefaultColor(ColorRGBA.white);
        background.setLocalTranslation(new Vector3f(width / 2f, height * aspect / 2f, 0.0f));

        TextureState texState = r.createTextureState();
        Texture tex = Setup.loadTextureUI("logo.tga", true);
        texState.setTexture(tex);
        background.setRenderState(texState);
        
        Quad background2 = new Quad("Background2", width / 2f, (height * aspect) / 2f );
        background2.setRenderQueueMode(Renderer.QUEUE_ORTHO);
        background2.setColorBuffer(null);
        background2.setDefaultColor(ColorRGBA.white);
        background2.setLocalTranslation(new Vector3f(width / 2f, height / 3f, 0.0f));

        texState = r.createTextureState();
        tex = Setup.loadTextureUI("loadingtext.tga", true);
        texState.setTexture(tex);
        background2.setRenderState(texState);
        
        Node menu = new Node("menu");
        menu.attachChild(background);
        menu.attachChild(background2);
        menu.setRenderState(as);
        
        menu.updateGeometricState(0, true);
        menu.updateRenderState();
        
        loading = new LoadingController(menu);
        menu.addController(loading);
        
        return menu;
    }
    
    public static void loadMusic(){
        URL trackURL = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_AUDIO, "game.ogg");
        AudioTrack track = AudioSystem.getSystem().createAudioTrack(trackURL, true);

        MusicTrackQueue queue = AudioSystem.getSystem().getMusicQueue();
        queue.setCrossfadeinTime(0);
        queue.setRepeatType(RepeatType.ONE);
        queue.addTrack(track);
        queue.play();
    }
    
    public static void doLoading(JmeContext cx){
        try{
            // set basic rendering states
            ZBufferState zbuf = cx.getRenderer().createZBufferState();
            zbuf.setFunction(TestFunction.LessThan);
            rootNode.setRenderState(zbuf);

            CullState cull = cx.getRenderer().createCullState();
            cull.setCullFace(CullState.Face.Back);
            rootNode.setRenderState(cull);

            File optFile = new File(System.getProperty("user.dir"), "/data/models/hank.jme");
            // load character
            ModelNode hank = CharacterLoader.loadCharacter(
                                    ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, "hank.md5mesh"), 
                                    ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, "hank.md5anim"),
                                    ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, "hank_walk.md5anim"),
                                    optFile,
                                    "hank");

            hank.setLocalTranslation(new Vector3f(0, 10, 0));

            Quaternion q = new Quaternion();
            q.fromAngles(-FastMath.HALF_PI, 0, 0);

            Spatial child = hank.getChild(0);
            child.setLocalScale(0.04f);
            child.setLocalRotation(q);
            child.setLocalTranslation(-0.65f, 0, 0);
            hank.updateModelBound();
            hank.updateWorldBound();

            // bind controls to character
            PlayerController pc = new PlayerController(hank, cx.getRenderer().getCamera(), walkableNode);
            hank.addController(pc);
            rootNode.attachChild(hank);

            Spatial terrain = null;
            try{
                terrain = loadTerrain(cx);
            } catch (IOException ex){
                ErrorReporter.reportError("IO Error while reading terrain", ex);
            }

            walkableNode.attachChild(terrain);

            // update input every frame
            inputPass = new InputPass(pc.getHandler(), false);

            // do not attach walkableNode to scene graph
            // because TerrainPass already renders the terrain in walkableNode
            rootNode.attachChild(walkableNode);

            LightState ls = cx.getRenderer().createLightState();
            PointLight light = new PointLight();
            light.setEnabled(true);
            light.setAmbient(ColorRGBA.darkGray);
            light.setDiffuse(new ColorRGBA(.9f, .9f, .7f, 1f));
            light.setSpecular(ColorRGBA.white);
            light.setLocation(new Vector3f(256,420,420));
            light.setShadowCaster(true);
            ls.attach(light);

            light = new PointLight();
            light.setEnabled(true);
            light.setAmbient(new ColorRGBA(.2f, .2f, .3f, 1f));
            light.setDiffuse(ColorRGBA.white);
            light.setSpecular(ColorRGBA.black);
            light.setLocation(new Vector3f(-400,100,-400));
            light.setShadowCaster(false);
            ls.attach(light);

            rootNode.setRenderState(ls);

            // load sky
            //Skybox sky = loadSkybox(cx);

            rootNode.updateGeometricState(0, true);
            rootNode.updateRenderState();

            skyPass = new RenderPass(PassType.PRE_RENDER, "sky");
            //skyPass.add(sky);

            renderPass = new RenderPass(PassType.RENDER, "scene");
            renderPass.add(rootNode);

            System.gc();
            loading.notifyComplete();
        } catch (Throwable t){
            ErrorReporter.reportError("Exception occured while loading", t);
        }
    }
    
    public static void doAttachScene(JmeContext cx){
        cx.getPassManager().add(inputPass);
        cx.getPassManager().add(skyPass);
        cx.getPassManager().add(renderPass);
    }
    
    public static void main(String[] args){
        Logger.getLogger("").addHandler(new ErrorReporter.ErrorHandler());
        Logger.getLogger("").setLevel(Level.WARNING);
        //System.setProperty("org.lwjgl.util.Debug", "true");
        
        try{
            Thread.sleep(500);
        }catch (InterruptedException ex){
        }
        
            JmeContext cx = Setup.loadDisplay();

            Setup.setupLocators();
            Setup.loadPasses(cx);

            // initialize sound
            // loading screen theme
            if (cx.getSettings().isMusic()){
                SoundPass sp = new SoundPass();
                AudioSystem.getSystem().setMasterGain(cx.getSettings().getFloat("GameMusicVolume", 1f));
                loadMusic();
                cx.getPassManager().add(sp);
            }

            cx.execute(new Callable<Void>(){
                public Void call(){
                    Thread.currentThread().setUncaughtExceptionHandler(new ErrorReporter.ErrorHandler());
                    SysInfo.querySystemInfo();
                    return null;
                }
            });
            
            Spatial menu = loadLoadingScreen(cx);
            // give a special name so that LoadingController can detach it

            RenderPass menuPass = new RenderPass(PassType.POST_RENDER, "menu");
            menuPass.add(menu);
            cx.getPassManager().add(menuPass);

        doLoading(cx);
    }
    
}
