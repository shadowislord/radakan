package com.radakan.game.tile;

import com.gibbon.jme.context.JmeContext;
import com.jme.image.Image;
import com.jme.image.Image.Format;
import com.jme.image.Texture;
import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MinificationFilter;
import com.jme.image.Texture.WrapMode;
import com.jme.image.Texture2D;
import com.jme.renderer.Renderer;
import com.jme.scene.state.GLSLShaderObjectsState;
import com.jme.scene.state.TextureState;
import com.jme.system.DisplaySystem;
import com.jme.util.geom.BufferUtils;

import com.radakan.graphics.terrain.AlphaDetailLayer;
import com.radakan.graphics.terrain.BaseLayer;
import com.radakan.graphics.terrain.LightLayer;
import com.radakan.graphics.terrain.SplatEnv;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;

public class TextureSet {

    public static final Format ALPHAMAP_FORMAT = Format.Alpha8;
    
    private List<Detailmap> detailmaps;
    private SplatEnv env;
    private GLSLShaderObjectsState glsl;
    private TextureState texState;
    private String name;
    
    private static Image nilImage;
    private static Texture2D nilTexture;
    
    
    
    static {
        ByteBuffer buf = BufferUtils.createByteBuffer(2 * 2 * Image.getEstimatedByteSize(ALPHAMAP_FORMAT));
        buf.put((byte)0xFF).put((byte)0xFF).put((byte)0x00).put((byte)0xFF);
        buf.rewind();
        nilImage = new Image(ALPHAMAP_FORMAT, 2, 2, buf);
        nilTexture = new Texture2D();
        nilTexture.setImage(nilImage);
        nilTexture.setStoreTexture(true);
//        nilTexture.setMagnificationFilter(MagnificationFilter.NearestNeighbor);
//        nilTexture.setMinificationFilter(MinificationFilter.NearestNeighborNoMipMaps);
        nilTexture.setMagnificationFilter(MagnificationFilter.Bilinear);
        nilTexture.setMinificationFilter(MinificationFilter.BilinearNoMipMaps);
        
        nilTexture.setWrap(WrapMode.Clamp);
    }
    
    public static class Detailmap {
        String name;
        Texture2D colormap;
        Texture2D normalmap;
        Texture2D specmap;
        float scale = 1f;
        
        public Detailmap(String name){
            this.name = name;
        }
        
        public void setDetail(Texture2D detail){
            colormap = detail;
        }
        
        @Override
        public String toString(){
            return name;
        }
        
    }
    
    public TextureSet(String name){
        this.name = name;
    }

    public void setName(String name){
        this.name = name;
    }
    
    @Override
    public String toString(){
        return name;
    }
    
    public List<Detailmap> getDetailmaps(){
        return detailmaps;
    }
    
    public static boolean isNilImage(Image image){
        return image == nilImage;
    }
    
    public void setDetailmaps(List<Detailmap> detailmaps){
        if (detailmaps.size() == 0)
            return;
        
        this.detailmaps = detailmaps;
        
        env = new SplatEnv();
        
        BaseLayer bl = new BaseLayer(detailmaps.get(0).colormap);
        bl.setScale(detailmaps.get(0).scale);
        env.addLayer(bl);
        
        for (int i = 1; i < detailmaps.size(); i++){
            Detailmap map = detailmaps.get(i);
            
            // it is possible to know which texture is an alphamap, if it is the nilTexture
            AlphaDetailLayer adl = new AlphaDetailLayer(map.colormap, nilTexture);
            env.addLayer(adl);
            adl.setScale(map.scale);
        }
        
        LightLayer ll = new LightLayer();
        env.addLayer(ll);
        
        JmeContext.get().executeLater(new Callable<Object>(){
            public Object call(){
                Renderer r = DisplaySystem.getDisplaySystem().getRenderer();
                glsl = env.createGLSLShader(r);
                TextureState[] states = env.createShaderPasses(r);
                
                texState = states[0];
                
                return null;
            }
        });
        
    }
    
    public Texture2D[] createStateCopy(TextureState state){
        List<Texture2D> alphamaps = new ArrayList<Texture2D>();
        
        for (int i = 0; i < texState.getNumberOfSetTextures(); i++){
            Texture texture = texState.getTexture(i);
            
            if (texture.getImage() == nilImage){
                // create another instance, except for the data which is shared
                // the data will be replaced when the alphamap is painted
                texture = texture.createSimpleClone();
                alphamaps.add((Texture2D) texture);
            }
            state.setTexture(texture, i);
        }
        
        return alphamaps.toArray(new Texture2D[0]);
    }
    
    public GLSLShaderObjectsState getShader(){
        return glsl;
    }
    
    /**
     * Applies this texture set to an object.
     * 
     * @param spatial
     * @return An array of all alpha textures assigned to the material
     */
//    public Texture2D[] applyMaterial(Spatial spatial){
//        List<Texture2D> alphamaps = new ArrayList<Texture2D>();
//        
//        // must make a copy of the texture state
//        // otherwise it cannot be modified
//        TextureState state = JmeContext.get().getRenderer().createTextureState();
//        
//        for (int i = 0; i < texState.getNumberOfSetTextures(); i++){
//            Texture texture = texState.getTexture(i);
//            
//            if (texture.getImage() == nilImage){
//                // create another instance, except for the data which is shared
//                // the data will be replaced when the alphamap is painted
//                texture = texture.createSimpleClone();
//                alphamaps.add((Texture2D) texture);
//            }
//            state.setTexture(texture, i);
//        }
//        
//        spatial.setRenderState(state);
//        spatial.setRenderState(glsl);
//        
//        spatial.updateRenderState();
//        
//        return alphamaps.toArray(new Texture2D[0]);
//    }
    
}
