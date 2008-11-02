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
package com.radakan.game.world;

import static com.radakan.util.XMLUtil.*;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.logging.Logger;

import org.w3c.dom.Node;

import com.jme.image.Image;
import com.jme.image.Texture;
import com.jme.image.Texture2D;
import com.jme.light.DirectionalLight;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.renderer.ColorRGBA;
import com.jme.renderer.Renderer;
import com.jme.scene.Skybox;
import com.jme.scene.Skybox.Face;
import com.jme.scene.state.FogState;
import com.jme.scene.state.LightState;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.FogState.CoordinateSource;
import com.jme.scene.state.FogState.DensityFunction;
import com.jme.scene.state.ZBufferState.TestFunction;
import com.jme.system.GameSettings;
import com.jme.util.TextureManager;
import com.jme.util.resource.ResourceLocatorTool;
import com.radakan.game.Game;
import com.radakan.game.camera.GameCameraManager;
import com.radakan.game.config.GameConfigManager;
import com.radakan.game.display.GameDisplayManager;
import com.radakan.game.entity.Entity;
import com.radakan.game.entity.GameEntityManager;
import com.radakan.game.entity.unit.ModelUnit;
import com.radakan.game.tile.Tile;
import com.radakan.game.tile.GameTileManager;

/**
 * @author Joshua Montgomery
 * @author Kirill Vainer
 * 
 * @version 1.5
 * @created Aug 3, 2008
 */
public class GameWorldManager extends com.jme.scene.Node {

    private static final Logger logger = Logger.getLogger(GameWorldManager.class.getName());
    
    private com.jme.scene.Node worldSky = null;
    private LightState worldLighting = null;
    private FogState worldFog = null;

    /**
     * Create a new World instance. 
     * All world features (sky, fog, lighting, terrain)
     * are disabled by default unless specified in the World.XML file.
     * 
     * Depends on:
     *  - GameDisplayManager
     *  - GameConfigManager
     *  - GameTileManager
     *  - GameCameraManager
     * 
     * @param renderer Renderer to which the world is rendered
     * @param gameSettings Presistent game settings to for quality controls
     */
    public GameWorldManager(){
        super("World Node");
        
        GameDisplayManager display = Game.getDisplayManager();
        GameConfigManager config = Game.getConfigManager();
        
        Renderer renderer = display.getContext().getRenderer();
        GameSettings settings = config.getSettings();
        
        // make sure to attach sky first
        worldSky = new com.jme.scene.Node("World Sky Node");
        worldSky.setRenderQueueMode(Renderer.QUEUE_SKIP);
        
        ZBufferState noWrite = renderer.createZBufferState();
        noWrite.setWritable(false);
        noWrite.setFunction(TestFunction.Always);
        worldSky.setRenderState(noWrite);
        attachChild(worldSky);
        
        worldLighting = renderer.createLightState();
        worldLighting.setSeparateSpecular(true);
        worldLighting.setTwoSidedLighting(false);
        worldLighting.setEnabled(false);
        setRenderState(worldLighting);
        
        worldFog = renderer.createFogState();
        worldFog.setEnabled(false);
        setRenderState(worldFog);
        
        GameTileManager tileMan = Game.getTileManager();
        attachChild(tileMan);
    }
    
    @Override
    public void updateGeometricState(float tpf, boolean initiator){
        GameCameraManager camMan = Game.getCameraManager();
        
        // make sure sky centered on camera before anything else is updated
        Camera cam = camMan.getCurrentCamera();
        worldSky.setLocalTranslation(cam.getLocation());
        
        super.updateGeometricState(tpf, initiator);
    }
    
    public void addEntity(Entity ent, Tile tile){
        ModelUnit model = ent.getModel();
        ent.birth();
        tile.addObject(model.getModel());
    }
    
    public void removeEntity(Entity ent, Tile tile){
        ModelUnit model = ent.getModel();
        tile.removeObject(model.getModel());
        ent.dispose();
    }
    
    /**
     * Load a sun (a directional light) from XML.
     * You can specify direction, and lighting colors (ambient, diffuse, specular)
     * for sun.
     * 
     * @param sunXMLNode
     */
    protected void loadSun(Node sunXMLNode){
        DirectionalLight dl = new DirectionalLight();
        dl.setEnabled(true);
        dl.setDirection(getVec3Attribute(sunXMLNode, "direction", null).normalizeLocal());
        dl.setAmbient(getRGBAAttribute(sunXMLNode, "ambient", ColorRGBA.darkGray));
        dl.setDiffuse(getRGBAAttribute(sunXMLNode, "diffuse", ColorRGBA.gray));
        dl.setSpecular(getRGBAAttribute(sunXMLNode, "specular", ColorRGBA.lightGray));
        dl.setShadowCaster(true);
        
        worldLighting.attach(dl);
    }
    
    /**
     * Loads lighting parameters from XML.
     * 
     * @param lightXMLNode
     */
    protected void loadLighting(Node lightXMLNode){
        worldLighting.setEnabled(true);
        
        Node sunXMLNode = lightXMLNode.getFirstChild();
        while (sunXMLNode != null){
            if (sunXMLNode.getNodeName().equals("sun")){
                loadSun(sunXMLNode);
            }else{
                logger.warning("Unknown light type specified under lighting: "+sunXMLNode.getNodeName());
            }
            
            sunXMLNode = sunXMLNode.getNextSibling();
        }
    }
    
    /**
     * Takes a cubemap texture and extracts an individual face (East, West, etc), turning it
     * into a single texture.
     * 
     * @param cubemap
     * @param imageN
     * @return
     */
    private static final Texture extractTextureFromCubemap(Texture cubemap, int imageN){
        Texture tex = new Texture2D();
        
        tex.setMinificationFilter(cubemap.getMinificationFilter());
        tex.setMagnificationFilter(cubemap.getMagnificationFilter());
        tex.setAnisotropicFilterPercent(cubemap.getAnisotropicFilterPercent());
        
        Image source = cubemap.getImage();
        Image img = new Image(source.getFormat(), 
                              source.getWidth(), 
                              source.getHeight(),
                              source.getData(imageN));
        tex.setImage(img);
        
        return tex;
    }
    
    /**
     * Loads a sky model from XML.
     * Currently only a static skybox is supported
     * with a DDS cubemap image.
     * 
     * @param skyXMLNode
     */
    protected void loadSky(Node skyXMLNode){
        String cubemapPath = getAttribute(skyXMLNode, "cubemap");
        
        if (cubemapPath == null){
            logger.warning("sky tag specified but missing cubemap attribute");
            return;
        }
        
        URL url = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_TEXTURE, cubemapPath);
        if (url == null)
            logger.warning("Cannot find cubemap "+cubemapPath);
        
        Texture cubemap = TextureManager.loadTexture(url, true);
        
        Skybox box = new Skybox("World Skybox", 2f, 2f, 2f);
        box.setTexture(Face.North, extractTextureFromCubemap(cubemap, 0));
        box.setTexture(Face.South, extractTextureFromCubemap(cubemap, 1));
        box.setTexture(Face.Up,    extractTextureFromCubemap(cubemap, 2));
        box.setTexture(Face.Down,  extractTextureFromCubemap(cubemap, 3));
        box.setTexture(Face.West,  extractTextureFromCubemap(cubemap, 4));
        box.setTexture(Face.East,  extractTextureFromCubemap(cubemap, 5));
        
        worldSky.attachChild(box);
    }
    
    /**
     * Loads fog parameters from XML.
     * Currently only the LINEAR density function is supported.
     * 
     * @param fogXMLNode
     */
    protected void loadFog(Node fogXMLNode){
        GameConfigManager config = Game.getConfigManager();
        
        float viewDistance = config.getSettings().getFloat("GameViewDistance", 150);
        float fogEnd = getFloatAttribute(fogXMLNode, "end", 1000f);
        
        // limit fog end by view distance, dont want fog leaking beyond view distance
        worldFog.setEnd(Math.min(viewDistance, fogEnd));
        worldFog.setStart(getFloatAttribute(fogXMLNode, "start", 100f));
        worldFog.setColor(getRGBAAttribute(fogXMLNode, "color", ColorRGBA.black));
        worldFog.setDensityFunction(DensityFunction.Linear);
        worldFog.setSource(CoordinateSource.Depth);
        worldFog.setEnabled(true);
    }
    
    /**
     * Loads tile parameters from XML and sets up the tile manager.
     * 
     * @param tilemanXMLNode
     */
    protected void loadTileManager(Node tilemanXMLNode){
        GameConfigManager config = Game.getConfigManager();
        
        boolean useLightmapsSetting = config.getSettings().getBoolean("GameLightmaps", false);
        boolean useLightmapsWorld   = getBoolAttribute(tilemanXMLNode, "uselightmaps");
        boolean useFog = worldFog.isEnabled();
        
        float viewDistance = config.getSettings().getFloat("GameViewDistance", 150f);
        
        GameTileManager tileManager = Game.getTileManager();
        // use lightmaps if they are included with the world and the game setting allows their use
        tileManager.setUseLightmaps(useLightmapsSetting && useLightmapsWorld);
        tileManager.setUseFog(useFog);
        tileManager.setTileSize(getIntAttribute(tilemanXMLNode, "tilesize", 64));
        tileManager.setGroupSize(getIntAttribute(tilemanXMLNode, "groupsize", 3));
        tileManager.setLoadDistance(viewDistance);
        tileManager.setUnloadDistance(viewDistance * 1.5f);
        tileManager.setEnabled(true);
        
        try {
            tileManager.loadDefaultTextureSets();
        } catch (IOException ex){
            Game.getDebugManager().reportError("Error while loading texturesets", ex);
        }
    }
    
    /**
     * Loads world settings from XML.
     * Settings allow activation of the following features:
     * Sunlight,
     * Sky,
     * Fog,
     * Camera/Eye params,
     * Terrain/Tile params.
     * 
     * @param worldXMLNode
     */
    protected void loadWorld(Node worldXMLNode){
        Node lightXMLNode = getChildNode(worldXMLNode, "lighting");
        Node skyXMLNode = getChildNode(worldXMLNode, "sky");
        Node fogXMLNode = getChildNode(worldXMLNode, "fog");
        Node cameraXMLNode = getChildNode(worldXMLNode, "camera");
        Node tilemanXMLNode = getChildNode(worldXMLNode, "tilemanager");
        
        if (lightXMLNode != null) 
            loadLighting(lightXMLNode);
        
        if (skyXMLNode != null)
            loadSky(skyXMLNode);
        
        if (fogXMLNode != null)
            loadFog(fogXMLNode);
        
//        if (cameraXMLNode != null)
//            loadCamera(cameraXMLNode);
        
        if (tilemanXMLNode != null)
            loadTileManager(tilemanXMLNode);
    }
    
    public void loadWorld(InputStream in) throws IOException{
        loadWorld(loadDocument(in, "world"));
    }
    
}
