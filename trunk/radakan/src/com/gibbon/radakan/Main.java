package com.gibbon.radakan;

import com.gibbon.jme.context.ConfigFrame;
import com.gibbon.jme.context.ExitListenerPass;
import com.gibbon.jme.context.InputPass;
import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.PassManager;
import com.gibbon.jme.context.PassType;
import com.gibbon.jme.context.RenderPass;
import com.gibbon.jme.context.SoundPass;
import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.gibbon.mfkarpg.terrain.splatting.CXTerrainPass;
import com.gibbon.mfkarpg.terrain.splatting.TerrainPass;
import com.gibbon.radakan.menu.LoadingController;
import com.gibbon.radakan.player.PlayerController;
import com.jme.bounding.BoundingBox;
import com.jme.bounding.CollisionTree;
import com.jme.bounding.CollisionTreeManager;
import com.jme.image.Image;
import com.jme.image.Texture;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.renderer.ColorRGBA;
import com.jme.renderer.Renderer;
import com.jme.scene.Node;
import com.jme.scene.Skybox;
import com.jme.scene.Spatial;
import com.jme.scene.shape.Quad;
import com.jme.scene.state.AlphaState;
import com.jme.scene.state.CullState;
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
import com.jmex.terrain.TerrainPage;
import com.jmex.terrain.util.RawHeightMap;
import com.model.md5.ModelNode;
import java.io.File;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URL;
import java.util.concurrent.Callable;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.prefs.Preferences;

public class Main {

    private static Node walkableNode = new Node("walkable");
    private static Node rootNode     = new Node("root");
    
    private static CXTerrainPass terrainPass;
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
        return TextureManager.loadTexture(name,Texture.MM_LINEAR_LINEAR,Texture.FM_LINEAR,0.0f,flipY);
    }
    
    /**
     * Loads an image but doesn't compress it
     * 
     * @param name
     * @param flipY
     * @return
     */
    public static Texture loadTextureUI(String name, boolean flipY){
        return TextureManager.loadTexture(name,Texture.MM_LINEAR_LINEAR,Texture.FM_LINEAR,Image.GUESS_FORMAT_NO_S3TC,0.0f,flipY);
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
        
        sky.setTexture(Skybox.DOWN,  loadTexture("sky_down.dds", false));
        sky.setTexture(Skybox.UP,    loadTexture("sky_up.dds", false));
        sky.setTexture(Skybox.EAST,  loadTexture("sky_east.dds", false));
        sky.setTexture(Skybox.WEST,  loadTexture("sky_west.dds", false));
        sky.setTexture(Skybox.NORTH, loadTexture("sky_north.dds", false));
        sky.setTexture(Skybox.SOUTH, loadTexture("sky_south.dds", false));
        
        sky.setRenderQueueMode(Renderer.QUEUE_SKIP);
        
        ZBufferState zbuf = cx.getRenderer().createZBufferState();
        zbuf.setFunction(ZBufferState.CF_ALWAYS);
        zbuf.setWritable(false);
        sky.setRenderState(zbuf);
        
        sky.updateRenderState();
        
        return sky;
    }
    
    public static CXTerrainPass loadTerrain(JmeContext cx){
        CollisionTreeManager.getInstance().setTreeType(CollisionTree.AABB_TREE);
        
        URL heights = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_TEXTURE, "spirits.int16le");
        RawHeightMap heightMap = new RawHeightMap(heights, 129, RawHeightMap.FORMAT_16BITLE, false);
        
        Vector3f terrainScale = new Vector3f(5, 0.003f, 6);
        heightMap.setHeightScale(0.001f);
        //MidPointHeightMap heightMap = new MidPointHeightMap(128, 1.9f);
        //Vector3f terrainScale = new Vector3f(5,1,5);
        //TerrainBlock terrain = new TerrainBlock("Terrain", heightMap.getSize(), terrainScale,
        //                                   heightMap.getHeightMap(),
        //                                   new Vector3f(0, 0, 0), false);
        TerrainPage terrain = new TerrainPage("Terrain", 33, heightMap.getSize(),
                terrainScale, heightMap.getHeightMap(), false);
        
        Texture lightmap = loadTexture("lightmap.tga", false);
        Texture basetex = loadTexture("baserock.dds", false);
        
        Texture alphatex0 = loadTexture("darkrockalpha3.tga", true);
        Texture alphatex1 = loadTexture("deadalpha3.tga", true);
        Texture alphatex2 = loadTexture("grassalpha3.tga", true);
        Texture alphatex3 = loadTexture("roadalpha3.tga", true);
        
        Texture detailtex0 = loadTexture("darkrock.dds", false);
        Texture detailtex1 = loadTexture("deadgrass.dds", false);
        Texture detailtex2 = loadTexture("nicegrass.dds", false);
        Texture detailtex3 = loadTexture("road.dds", false);

        // Create the terrain pass
        CXTerrainPass tPass = new CXTerrainPass();
        tPass.setRenderMode(TerrainPass.MODE_BEST);
        
        // Add a terrain model to the pass
        tPass.addTerrain(terrain);
        
        tPass.setTileScale(90);
        
        // Add the tiles
        tPass.addDetail(basetex,null);
        tPass.addDetail(detailtex0,alphatex0);
        tPass.addDetail(detailtex1,alphatex1);
        tPass.addDetail(detailtex2,alphatex2);
        tPass.addDetail(detailtex3,alphatex3);
        tPass.setLightmap(lightmap, 2.0f);
        
        ZBufferState zbuf = cx.getRenderer().createZBufferState();
        zbuf.setFunction(ZBufferState.CF_LESS);
        tPass.setPassState(zbuf);
        
        CullState cull = cx.getRenderer().createCullState();
        cull.setCullMode(CullState.CS_BACK);
        tPass.setPassState(cull);
        
        terrain.updateRenderState();
        
        terrain.setModelBound(new BoundingBox());
        terrain.updateModelBound();
        //test protection
        CollisionTreeManager.getInstance().generateCollisionTree(CollisionTree.AABB_TREE, terrain, true);
        
        walkableNode.attachChild(terrain);
        
        return tPass;
    }
    
    public static JmeContext loadDisplay(){
        try {
            GameSettings gs = new PreferencesGameSettings(Preferences.userRoot().node("Radakan"));
            ConfigFrame config = new ConfigFrame(gs);
            config.setVisible(true);
            config.waitFor();

            // set custom settings here
            gs.set("title", "Radakan Prototype 0.05");
            gs.setSamples(0);
            gs.setFramerate(60);

            JmeContext cx = JmeContext.create(LWJGLContext.class, JmeContext.CONTEXT_WINDOW);
            cx.setSettings(gs);
            cx.start();
            cx.waitFor();

            return cx;
        } catch (InstantiationException ex) {
            Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
        } catch (InterruptedException ex) {
            Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
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
        Texture tex = loadTextureUI("logo.png", true);
        texState.setTexture(tex);
        background.setRenderState(texState);
        
        
        Quad background2 = new Quad("Background2", width / 2f, (height * aspect) / 2f );
        background2.setRenderQueueMode(Renderer.QUEUE_ORTHO);
        background2.setColorBuffer(0, null);
        background2.setDefaultColor(ColorRGBA.white);
        background2.setLocalTranslation(new Vector3f(width / 2f, height / 3f, 0.0f));

        texState = r.createTextureState();
        tex = loadTextureUI("loadingtext.png", true);
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
        // set basic rendering states
        ZBufferState zbuf = cx.getRenderer().createZBufferState();
        zbuf.setFunction(ZBufferState.CF_LESS);
        rootNode.setRenderState(zbuf);
        
        CullState cull = cx.getRenderer().createCullState();
        cull.setCullMode(CullState.CS_BACK);
        rootNode.setRenderState(cull);
        
        System.out.println("1");
        
        // load character
        ModelNode hank = CharacterLoader.loadCharacter(
                                ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, "hank.md5mesh"), 
                                ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, "hank.md5anim"),
                                ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, "hank_walk.md5anim"),
                                "hank");
        
        hank.setLocalTranslation(new Vector3f(-213, 0, -120));
        
        Quaternion q = new Quaternion();
        q.fromAngles(-FastMath.HALF_PI, 0, 0);
        
        Spatial child = hank.getChild(0);
        child.setLocalScale(0.25f);
        child.setLocalRotation(q);
        child.setLocalTranslation(-3.5f, 0, 0);
        
        System.out.println("2");
        
        // bind controls to character
        PlayerController pc = new PlayerController(hank, cx.getRenderer().getCamera(), walkableNode);
        hank.addController(pc);
        rootNode.attachChild(hank);
        
        System.out.println("3");
        
        // update input every frame
        inputPass = new InputPass(pc.getHandler(), false);
        
        // do not attach walkableNode to scene graph
        // because TerrainPass already renders the terrain in walkableNode
        //rootNode.attachChild(walkableNode);
        
        System.out.println("4");
        
        // load sky
        Skybox sky = loadSkybox(cx);

        System.out.println("5");
        
        rootNode.updateGeometricState(0, true);
        rootNode.updateRenderState();
        
        System.out.println("6");
        
        skyPass = new RenderPass(PassType.PRE_RENDER, "sky");
        skyPass.add(sky);
        
        renderPass = new RenderPass(PassType.RENDER, "scene");
        renderPass.add(rootNode);
        
        System.out.println("7");
        
        terrainPass = loadTerrain(cx);
        
        System.out.println("8");
        
        loading.notifyComplete();
    }
    
    public static void doAttachScene(JmeContext cx){
        cx.getPassManager().add(inputPass);
        cx.getPassManager().add(skyPass);
        cx.getPassManager().add(renderPass);
        cx.getPassManager().add(terrainPass);
    }
    
    public static void main(String[] args){
        Logger.getLogger("").setLevel(Level.WARNING);
        
        JmeContext cx = loadDisplay();
        
        setupLocators();
        loadPasses(cx);
   
        // initialize sound
        // loading screen theme
        SoundPass sp = new SoundPass();
        loadMusic();
        cx.getPassManager().add(sp);
        
        Spatial menu = loadMenu(cx);
        // give a special name so that LoadingController can detach it
        RenderPass menuPass = new RenderPass(PassType.POST_RENDER, "menu");
        menuPass.add(menu);
        cx.getPassManager().add(menuPass);
        
        doLoading(cx);
    }
    
}
