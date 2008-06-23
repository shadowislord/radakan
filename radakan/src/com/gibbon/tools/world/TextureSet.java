package com.gibbon.tools.world;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.mfkarpg.terrain.splatting.AlphaDetailLayer;
import com.gibbon.mfkarpg.terrain.splatting.BaseLayer;
import com.gibbon.mfkarpg.terrain.splatting.Layer;
import com.gibbon.mfkarpg.terrain.splatting.LightLayer;
import com.gibbon.mfkarpg.terrain.splatting.SplatEnv;
import com.jme.image.Image;
import com.jme.image.Image.Format;
import com.jme.image.Texture;
import com.jme.image.Texture.ApplyMode;
import com.jme.image.Texture.CombinerFunctionAlpha;
import com.jme.image.Texture.CombinerFunctionRGB;
import com.jme.image.Texture.CombinerOperandAlpha;
import com.jme.image.Texture.CombinerOperandRGB;
import com.jme.image.Texture.CombinerSource;
import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MinificationFilter;
import com.jme.image.Texture2D;
import com.jme.scene.Spatial;
import com.jme.scene.state.GLSLShaderObjectsState;
import com.jme.scene.state.TextureState;
import com.jme.util.geom.BufferUtils;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;

public class TextureSet {

    private List<Detailmap> detailmaps;
    private SplatEnv env;
    private GLSLShaderObjectsState glsl;
    private TextureState texState;
    private String name;
    
    private static Image nilImage;
    private static Texture2D nilTexture;
    
    static {
        ByteBuffer buf = BufferUtils.createByteBuffer(2 * 2 * Image.getEstimatedByteSize(TileGroup.ALPHAMAP_FORMAT));
        nilImage = new Image(TileGroup.ALPHAMAP_FORMAT, 2, 2, buf);
        nilTexture = new Texture2D();
        nilTexture.setImage(nilImage);
        nilTexture.setStoreTexture(true);
        //nilTexture.setMagnificationFilter(MagnificationFilter.NearestNeighbor);
        //nilTexture.setMinificationFilter(MinificationFilter.NearestNeighborNoMipMaps);
        nilTexture.setMagnificationFilter(MagnificationFilter.Bilinear);
        nilTexture.setMinificationFilter(MinificationFilter.BilinearNoMipMaps);
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
        bl.setScaleOverride(detailmaps.get(0).scale);
        env.addLayer(bl);
        
        for (int i = 1; i < detailmaps.size(); i++){
            Detailmap map = detailmaps.get(i);
            
            // it is possible to know which texture is an alphamap, if it is the nilTexture
            AlphaDetailLayer adl = new AlphaDetailLayer(map.colormap, nilTexture);
            env.addLayer(adl);
            adl.setScaleOverride(map.scale);
        }
        
        LightLayer ll = new LightLayer();
        env.addLayer(ll);
        
        JmeContext.get().executeLater(new Callable<Object>(){
            public Object call(){
                glsl = env.createGLSLShader(JmeContext.get().getRenderer());
                TextureState[] states = env.createShaderPasses(JmeContext.get().getRenderer());
                
                if (states.length > 1)
                    System.out.println("PROBLEMS");
                
                texState = states[0];
                
                return null;
            }
        });
        
    }
    
    /**
     * Applies this texture set to an object.
     * 
     * @param spatial
     * @return An array of all alpha textures assigned to the material
     */
    public Texture2D[] applyMaterial(Spatial spatial){
        List<Texture2D> alphamaps = new ArrayList<Texture2D>();
        
        // must make a copy of the texture state
        // otherwise it cannot be modified
        TextureState state = JmeContext.get().getRenderer().createTextureState();
        
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
        
        // FIXME: Yes
//        state = JmeContext.get().getRenderer().createTextureState();
//        Texture2D tex = alphamaps.get(1);
//        tex.setApply(ApplyMode.Combine);
//        tex.setCombineFuncRGB(CombinerFunctionRGB.Replace);
//        tex.setCombineFuncAlpha(CombinerFunctionAlpha.Replace);
//        tex.setCombineSrc0RGB(CombinerSource.CurrentTexture);
//        tex.setCombineOp0RGB(CombinerOperandRGB.SourceAlpha);
//        tex.setCombineSrc0Alpha(CombinerSource.PrimaryColor);
//        tex.setCombineOp0Alpha(CombinerOperandAlpha.SourceAlpha);
//        
//        state.setTexture(tex);
        
        spatial.setRenderState(state);
        spatial.setRenderState(glsl);
        
        spatial.updateRenderState();
        
        System.out.println("SET STATE: "+spatial);
        
        return alphamaps.toArray(new Texture2D[0]);
    }
    
}
