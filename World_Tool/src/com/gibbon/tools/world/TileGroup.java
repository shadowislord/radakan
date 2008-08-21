package com.gibbon.tools.world;

import com.gibbon.jme.context.JmeContext;
import com.jme.image.Image;
import com.jme.image.Image.Format;
import com.jme.image.Texture2D;
import com.jme.math.Vector2f;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import com.jme.scene.state.GLSLShaderObjectsState;
import com.jme.scene.state.LightState;
import com.jme.scene.state.RenderState;
import com.jme.scene.state.TextureState;
import com.jme.util.export.InputCapsule;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.jme.util.export.OutputCapsule;
import com.jme.util.export.Savable;
import com.jme.util.geom.BufferUtils;
import com.radakan.game.tile.TextureSet;

import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * Changes recently made by Tomygun:
 *  added a few checks for null alphamaps and texSet when reading and writing
 */
public class TileGroup extends Node implements Savable {

    private int x, y;
    private Texture2D[] alphamaps;
    private TextureSet texSet = null;
    
    private TextureState textureState;
    private GLSLShaderObjectsState shaderState;
    
    public static final Format ALPHAMAP_FORMAT = Format.Alpha8;
    
    public static Image createPaintableImage(int width, int height){
        ByteBuffer data = BufferUtils.createByteBuffer(Image.getEstimatedByteSize(ALPHAMAP_FORMAT) * width * height);
        Image image = new Image(ALPHAMAP_FORMAT, width, height, data);
        
        return image;
    }
    
    public TileGroup(int x, int y){
        super("GROUP_"+x+"_"+y);
        this.x = x;
        this.y = y;
        
        setTextureCombineMode(TextureCombineMode.Replace);
    }
    
    public TileGroup(){
        setTextureCombineMode(TextureCombineMode.Replace);
    }
    
    @Override
    public void read(JMEImporter im) throws IOException {
        super.read(im);
        
        InputCapsule cap = im.getCapsule(this);
        x = cap.readInt("GroupX", 0);
        y = cap.readInt("GroupY", 0);
        texSet = EditorState.texsetMap.get(cap.readString("TextureSet", null));
        Object[] maps = cap.readSavableArray("Alphamaps", null);
		
		if(maps != null){
			alphamaps = new Texture2D[maps.length];
			for (int i = 0; i < maps.length; i++){
				alphamaps[i] = (Texture2D) maps[i];
			}
		}
        setTextureSet(texSet, false);
        
        setName("GROUP_"+x+"_"+y);
    }
    
    @Override
    public void write(JMEExporter ex) throws IOException {
        // set STORE TEXTURE before writing the NODE
        // otherwise image will NOT be stored
		if(alphamaps != null){
			for (Texture2D alphamap : alphamaps){
				alphamap.setStoreTexture(true);
			}
		}
        
        super.write(ex);
        
        OutputCapsule cap = ex.getCapsule(this);
        cap.write(x, "GroupX", 0);
        cap.write(y, "GroupY", 0);
		if(texSet != null){
			cap.write(texSet.toString(), "TextureSet", null);
		}
		if(alphamaps != null){
			cap.write(alphamaps, "Alphamaps", null);
		}
    }
    
    public boolean isTextureSet(){
        return alphamaps != null;
    }
    
    public TextureSet getTextureSet(){
        return texSet;
    }
    
    public TextureState getTerrainTextureState(){
        return textureState;
    }
    
    public GLSLShaderObjectsState getTerrainShader(){
        return shaderState;
    }
    
    public void applyLightmap(){
        Image lightmap = generateLightmap((LightState)World.getWorld().getRenderState(RenderState.RS_LIGHT));
        Texture2D lighttex = new Texture2D();
//        lighttex.setMinificationFilter(MinificationFilter.BilinearNoMipMaps);
//        lighttex.setMagnificationFilter(MagnificationFilter.Bilinear);
        lighttex.setImage(lightmap);
        
        TextureState state = JmeContext.get().getRenderer().createTextureState();
        state.setTexture(lighttex);
        
        for (Spatial child: children){
            TriMesh mesh = ((Tile) child).getTerrain();
            mesh.clearRenderState(RenderState.RS_GLSL_SHADER_OBJECTS);
            mesh.setLightCombineMode(LightCombineMode.Off);
            mesh.setRenderState(state);
            mesh.updateRenderState();
        }
    }
    
    public Image generateLightmap(LightState lighting){
        final int res = World.getWorld().getGroupmapResolution();
        final float groupWidth = World.getWorld().getGroupSize() * World.getWorld().getTileSize();
        
        // image in RGB8 format, because lights have colors too
        ByteBuffer buf = BufferUtils.createByteBuffer(res * res * 3);
        buf.rewind();
        Image img = new Image(Format.RGB8, res, res, buf);

        // temporary vectors for lighting equations
        ColorRGBA tempC = new ColorRGBA();
        Vector3f tempV = new Vector3f();
        
        // result color that is written to the pixel
        ColorRGBA result = new ColorRGBA();
        
        for (int py = 0; py < res; py++){
            for (int px = 0; px < res; px++){
                // find pixel in world space
                Vector2f UV = new Vector2f((float)px / res, (float)py / res);
                UV.negateLocal();
                UV.subtractLocal(getX(), getY());
                UV.multLocal(groupWidth);
                
                Vector3f point = new Vector3f(UV.x, 0.0f, UV.y);
                Vector3f normal = new Vector3f();
                
                // find height and normal at that point
                float h = PickUtils.getTerrainHeight(this, point, normal);
                point.y = h + 1.0f;
                if (Float.isNaN(h))
                    continue;
                
                // vector "point" is the position of the pixel in world space now
                // -- calculate lighting equation for point --
                
                // reset result color
                result.set(ColorRGBA.black);
//                result.set( (normal.x + 1.0f) / 2.0f, 
//                            (normal.y + 1.0f) / 2.0f, 
//                            (normal.z + 1.0f) / 2.0f, 
//                             1.0f);
                result.set(h / 20f, h / 20f, h / 20f, 1.0f);
                result.clamp();
                
                /*for (Light light : lighting.getLightList()){
                    // C = (L . N) * Nd + Na
                    
                    if (light instanceof DirectionalLight){
                        DirectionalLight dl = (DirectionalLight) light;
                        
                        Ray r = new Ray(point, dl.getDirection().negate());
                        TriMesh collided = (TriMesh) PickUtils.calculatePick(World.getWorld(), r, tempV, false);
                        tempC.set(ColorRGBA.black);
                        if (collided == null){
                            tempC.addLocal(ColorRGBA.gray);
                        }
//                        float NdotL = dl.getDirection().dot(normal);
//                        if (NdotL <= 0.0f)
//                            continue;
                        
//                        tempC.set( (normal.x + 1.0f) / 2.0f, 
//                                   (normal.y + 1.0f) / 2.0f, 
//                                   (normal.z + 1.0f) / 2.0f, 
//                                    1.0f);
                        // temp = Nd * NdotL + Na
//                        tempC.set(dl.getDiffuse()).multLocal(NdotL).addLocal(dl.getAmbient());
                        tempC.clamp();
                        result.addLocal(tempC);
                    }/*else if (light instanceof PointLight){
                        PointLight pl = (PointLight) light;
                        // tempV = normalize(light_pos - point)
                        tempV.set(pl.getLocation()).subtractLocal(point).normalizeLocal();
                        Ray r = new Ray(point, tempV.clone());
                        TriMesh collided = (TriMesh) PickUtils.calculatePick(World.getWorld(), r, tempV, false);
                        tempC.set(pl.getAmbient());
                        if (collided == null){
                            tempC.addLocal(pl.getDiffuse());
                        }
                        
                        //float NdotL = tempV.dot(normal);
                        // tempC = Nd * NdotL + Na
                        //tempC.set(pl.getDiffuse()).multLocal(NdotL).addLocal(pl.getAmbient());
                        result.addLocal(tempC);
                    }
                }*/
                
                // convert result to bytes..
                buf.put( (byte)(((int)(result.r * 255)) & 0xFF) );
                buf.put( (byte)(((int)(result.g * 255)) & 0xFF) );
                buf.put( (byte)(((int)(result.b * 255)) & 0xFF) );
            }
        }
        buf.rewind();
        
        return img;
    }
    
    public void setTextureSet(TextureSet set, boolean replaceAlphamaps){
        if (set == null)
            return;
        
        texSet = set;
        
        textureState = JmeContext.get().getRenderer().createTextureState();
        shaderState = set.getShader();
        
        if (replaceAlphamaps){
            alphamaps = set.createStateCopy(textureState);
            if (alphamaps == null)
                throw new IllegalStateException("FAILED TO REPLACE ALPHAMAPS");
        }else{
            // get list of dummy alphamaps
            Texture2D[] tempMaps = set.createStateCopy(textureState);
            
            // replace with this tilegroup's alphamaps
            for (int i = 0; i < tempMaps.length; i++){
                tempMaps[i].setImage(alphamaps[i].getImage());
            }
            alphamaps = tempMaps;
            
            if (alphamaps == null)
                throw new IllegalStateException("FAILED TO RELOCATE ALPHAMAPS");
        }
        
        if (children == null)
            return;
        
        for (Spatial child: children){
            TriMesh mesh = ((Tile) child).getTerrain();
            mesh.setRenderState(shaderState);
            mesh.setRenderState(textureState);
            mesh.updateRenderState();
        }
    }
    
    public Texture2D getPaintableAlphamap(int i){
        Texture2D alphamap = alphamaps[i];

        if (TextureSet.isNilImage(alphamap.getImage())){
            // replace with paintable data
            int resolution = World.getWorld().getGroupmapResolution();
            Image image = createPaintableImage(resolution, resolution);
            alphamap.setImage(image);
        }
        
        return alphamap;
    }
    
    public int getAlphamapCount(){
        return alphamaps.length;
    }
    
    public Tile findTile(int x, int y){
        return (Tile) getChild("TILE_"+x+"_"+y);
    }
    
    public int getX() {
        return x;
    }
    
    public int getY() {
        return y;
    }
    
}
