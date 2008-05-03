/*
 * Radakan RPG is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan RPG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan RPG.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.gibbon.radakan;

import com.gibbon.jme.context.ExitListenerPass;
import com.gibbon.jme.context.InputPass;
import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.PassManager;
import com.gibbon.jme.context.PassType;
import com.gibbon.jme.context.RenderPass;
import com.gibbon.jme.context.SoundPass;
import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.gibbon.meshparser.MaterialLoader;
import com.gibbon.meshparser.SceneGraphDump;
import com.gibbon.radakan.config.ConfigFrame;
import com.gibbon.radakan.error.ErrorReporter;
import com.gibbon.radakan.menu.LoadingController;
import com.gibbon.radakan.player.PlayerController;
import com.gibbon.radakan.tile.TileLoader;
import com.gibbon.radakan.tile.TypeLoader;
import com.jme.bounding.BoundingBox;
import com.jme.bounding.CollisionTree;
import com.jme.bounding.CollisionTreeManager;
import com.jme.image.Image;
import com.jme.image.Texture;
import com.jme.light.PointLight;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.renderer.ColorRGBA;
import com.jme.renderer.Renderer;
import com.jme.scene.Node;
import com.jme.scene.Skybox;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import com.jme.scene.VBOInfo;
import com.jme.scene.shape.Quad;
import com.jme.scene.state.AlphaState;
import com.jme.scene.state.CullState;
import com.jme.scene.state.LightState;
import com.jme.scene.state.TextureState;
import com.jme.scene.state.ZBufferState;
import com.jme.system.GameSettings;
import com.jme.system.PreferencesGameSettings;
import com.jme.util.TextureManager;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import com.jmex.audio.AudioSystem;
import com.jmex.audio.AudioTrack;
import com.jmex.audio.MusicTrackQueue;
import com.jmex.audio.MusicTrackQueue.RepeatType;
import com.model.md5.ModelNode;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.net.URI;
import java.net.URL;
import java.util.concurrent.Callable;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.prefs.Preferences;

public class Main {

    private static Node walkableNode = new Node("walkable");
    private static Node rootNode     = new Node("root");
    
    private static RenderPass    renderPass;
    private static RenderPass    skyPass;
    private static InputPass     inputPass;
    
    private static LoadingController loading;
    
    public static void setupLocators(){
            String root = System.getProperty("user.dir");
            
            // setup audio locators
            URI musicDir = new File(root, "/data/music/").toURI();
            SimpleResourceLocator audio = new SimpleResourceLocator(musicDir);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_AUDIO, audio);

            // setup texture locators
            URI terDir = new File(root, "/data/terrain/").toURI();
            URI skyDir = new File(root, "/data/sky/").toURI();
            URI texDir = new File(root, "/data/textures/").toURI();

            SimpleResourceLocator ter = new SimpleResourceLocator(terDir);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, ter);
            SimpleResourceLocator tex = new SimpleResourceLocator(texDir);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, tex);
            SimpleResourceLocator sky = new SimpleResourceLocator(skyDir);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, sky);
            
            // setup shader locators
            URI shaderDir = new File(root, "/data/shaders/").toURI();
            SimpleResourceLocator shader = new SimpleResourceLocator(shaderDir);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_SHADER, shader);
            
            // setup model locators
            URI modelDir = new File(root, "/data/models/").toURI();
            SimpleResourceLocator model = new SimpleResourceLocator(modelDir);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_MODEL, model);
    }
    
    public static Texture loadTexture(String name, boolean flipY){
        return TextureManager.loadTexture(name,Texture.MM_LINEAR_LINEAR,Texture.FM_LINEAR,1.0f,flipY);
    }
    
    /**
     * Loads an image but doesn't compress it
     * 
     * @param name
     * @param flipY
     * @return
     */
    public static Texture loadTextureUI(String name, boolean flipY){
        return TextureManager.loadTexture(name,Texture.MM_NONE, Texture.FM_LINEAR,Image.GUESS_FORMAT_NO_S3TC,0.0f,flipY);
    }
    
    public static Skybox loadSkybox(final JmeContext cx){
        Skybox sky = new Skybox("sky", 2f, 2f, 2f){
            @Override
            public void updateGeometricState(float tpf, boolean initiator){
                Camera cam = cx.getRenderer().getCamera();
                setLocalTranslation(cam.getLocation());
                super.updateGeometricState(tpf, initiator);
            }
        };
        
//        sky.setTexture(Skybox.DOWN,  loadTexture("sky_down.dds", false));
//        sky.setTexture(Skybox.UP,    loadTexture("sky_up.dds", false));
//        sky.setTexture(Skybox.EAST,  loadTexture("sky_east.dds", false));
//        sky.setTexture(Skybox.WEST,  loadTexture("sky_west.dds", false));
//        sky.setTexture(Skybox.NORTH, loadTexture("sky_north.dds", false));
//        sky.setTexture(Skybox.SOUTH, loadTexture("sky_south.dds", false));
        
        sky.setTexture(Skybox.DOWN,  loadTexture("cloud_down.jpg", true));
        sky.setTexture(Skybox.UP,    loadTexture("cloud_up.jpg", true));
        
        sky.setTexture(Skybox.WEST,  loadTexture("cloud_1.jpg", true));
        sky.setTexture(Skybox.NORTH,  loadTexture("cloud_2.jpg", true));
        sky.setTexture(Skybox.EAST, loadTexture("cloud_4.jpg", true));
        sky.setTexture(Skybox.SOUTH, loadTexture("cloud_3.jpg", true));
        
        sky.setRenderQueueMode(Renderer.QUEUE_SKIP);
        
        ZBufferState zbuf = cx.getRenderer().createZBufferState();
        zbuf.setFunction(ZBufferState.CF_ALWAYS);
        zbuf.setWritable(false);
        sky.setRenderState(zbuf);
        
        sky.updateRenderState();
        
        return sky;
    }
    
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
                CollisionTreeManager.getInstance().generateCollisionTree(CollisionTree.AABB_TREE, mesh, true);
            //}
        }
    }
    
    public static Spatial loadTerrain(JmeContext cx) throws IOException{
        CollisionTreeManager.getInstance().setTreeType(CollisionTree.AABB_TREE);
        
//        URL bootstrap = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, "bootstrap.jme");
//        if (bootstrap == null)
//            throw new IllegalStateException("Cannot find world file");
//        
//        final Spatial world = (Spatial) BinaryImporter.getInstance().load(bootstrap);
        
        SimpleResourceLocator srl = new SimpleResourceLocator(new File("D:\\TileStore2\\").toURI());
        ResourceLocatorTool.addResourceLocator("tile", srl);
        ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_MODEL, srl);
        ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, srl);
        
        MaterialLoader mloader = new MaterialLoader();
        mloader.load(new FileInputStream("D:\\TileStore2\\area.material"));
        
        TypeLoader tloader = new TypeLoader();
        tloader.load(new FileInputStream("D:\\TileStore2\\area_types.xml"));
        
        TileLoader loader = new TileLoader(true);
        loader.setTypes(tloader.getTypeMap());
        loader.setMaterials(mloader.getMaterials());
        
        final Node world = new Node("WORLD");
        
        Spatial tile = loader.loadTile(0, 0);
        world.attachChild(tile);
        
        tile = loader.loadTile(-1, 0);
        tile.setLocalTranslation(64, 0, 0);
        world.attachChild(tile);
        
        tile = loader.loadTile(0, -1);
        tile.setLocalTranslation(0, 0, -64);
        world.attachChild(tile);
        
        tile = loader.loadTile(-1, -1);
        tile.setLocalTranslation(64, 0, -64);
        world.attachChild(tile);
        
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
    
    public static JmeContext loadDisplay(){
        try {
            GameSettings gs = new PreferencesGameSettings(Preferences.userRoot().node("Radakan"));
            ConfigFrame config = new ConfigFrame(gs);
            config.setVisible(true);
            config.requestFocus();
            config.waitFor();

            // set custom settings here
            gs.set("title", Radakan.getGameName() + " " + Radakan.getVersionPrefix() + " " + Radakan.getGameVersion());
            gs.setSamples(0);
            gs.setFramerate(60);
            gs.setVerticalSync(true);
            
            final JmeContext cx = JmeContext.create(LWJGLContext.class, JmeContext.CONTEXT_WINDOW);
            cx.setSettings(gs);
            cx.start();
            cx.waitFor();
            
            return cx;
        } catch (InstantiationException ex) {
            ErrorReporter.reportError("Failed to initialize Display implementor", ex);
        } catch (InterruptedException ex) {
            ErrorReporter.reportError("Recieved interrupt while waiting for Display", ex);
        }
        
        return null;
    }
    
    public static void loadPasses(JmeContext cx){
        final PassManager pm = cx.getPassManager();
        pm.execute(new Callable<Object>(){
            public Object call(){
                pm.add(new ExitListenerPass(
                        new Callable<Object>(){
                            public Object call(){
                                JmeContext.get().dispose();
                                System.exit(0);
                                return null;
                            }
                        }));
                return null;
            }
        });
    }
    
    public static Spatial loadMenu(JmeContext cx){
        Renderer r = cx.getRenderer();
        
        float width = r.getWidth();
        float height = r.getHeight();
        
        // XXX: WTF? Why do I have to multiply height by aspect
        //      to get correct results.. so random
        float aspect = height / width;
        
        AlphaState as = r.createAlphaState();
        as.setBlendEnabled(true);
        as.setSrcFunction(AlphaState.SB_SRC_ALPHA);
        as.setDstFunction(AlphaState.DB_ONE_MINUS_SRC_ALPHA);

        Quad background = new Quad("Background", width, height * aspect );
        background.setRenderQueueMode(Renderer.QUEUE_ORTHO);
        background.setColorBuffer(0, null);
        background.setDefaultColor(ColorRGBA.white);
        background.setLocalTranslation(new Vector3f(width / 2f, height * aspect / 2f, 0.0f));

        TextureState texState = r.createTextureState();
        Texture tex = loadTextureUI("logo.tga", true);
        texState.setTexture(tex);
        background.setRenderState(texState);
        
        Quad background2 = new Quad("Background2", width / 2f, (height * aspect) / 2f );
        background2.setRenderQueueMode(Renderer.QUEUE_ORTHO);
        background2.setColorBuffer(0, null);
        background2.setDefaultColor(ColorRGBA.white);
        background2.setLocalTranslation(new Vector3f(width / 2f, height / 3f, 0.0f));

        texState = r.createTextureState();
        tex = loadTextureUI("loadingtext.tga", true);
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
            zbuf.setFunction(ZBufferState.CF_LESS);
            rootNode.setRenderState(zbuf);

            CullState cull = cx.getRenderer().createCullState();
            cull.setCullMode(CullState.CS_BACK);
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
        
            JmeContext cx = loadDisplay();

            setupLocators();
            loadPasses(cx);

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
                    Radakan.querySystemInfo();
                    return null;
                }
            });
            
            Spatial menu = loadMenu(cx);
            // give a special name so that LoadingController can detach it

            RenderPass menuPass = new RenderPass(PassType.POST_RENDER, "menu");
            menuPass.add(menu);
            cx.getPassManager().add(menuPass);

        doLoading(cx);
    }
    
}
