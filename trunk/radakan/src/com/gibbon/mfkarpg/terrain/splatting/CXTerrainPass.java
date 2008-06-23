package com.gibbon.mfkarpg.terrain.splatting;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.PassType;
import com.gibbon.jme.context.RenderPass;
import com.jme.image.Texture;
import com.jme.image.Texture.CombinerScale;
import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MinificationFilter;
import com.jme.renderer.RenderContext;
import com.jme.renderer.Renderer;
import com.jme.scene.Geometry;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.state.*;
import com.jme.util.TextureManager;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

public class CXTerrainPass extends RenderPass {

    public static final int MODE_BEST = -1,
                            MODE_DEFAULT = -1,
                            MODE_FIXED_FUNC = 0,
                            MODE_FRAG_PROGRAM = 1,
                            MODE_GLSL = 2;
    
    protected SplatEnv env = new SplatEnv();
    
    protected BaseLayer base = null;
    protected List<AlphaDetailLayer> detail = new ArrayList<AlphaDetailLayer>();
    protected LightLayer light = null;
    protected boolean lightmap = false;
    protected FogLayer fog = null;
    
    protected int renderMethod = -1;
    protected int fallback = MODE_DEFAULT;
    
    protected List<RenderState[]> passList = new ArrayList<RenderState[]>();
    
    public CXTerrainPass(){
        super(PassType.RENDER, "terrain");
    }
    
    /**
     * Sets the rendering mode
     * <br>
     * Possible values:<br>
     *   MODE_BEST          - Uses the best supported rendering mode (GLSL, then frag program, then fixed func).<br>
     *   MODE_DEFAULT       - Uses the default rendering mode. This is the same as MODE_BEST
     *   MODE_FIXED_FUNC    - Uses fixed function rendering mode. Requires more passes than other modes, but is the most compatible.<br>
     *   MODE_FRAG_PROGRAM  - Uses ARB fragment program.<br>
     *   MODE_GLSL          - Uses GLSL shader.<br>
     */
    public void setRenderMode(int mode){
        if (mode < MODE_BEST || mode > MODE_GLSL)
            throw new IllegalArgumentException("Invalid mode specified");
        
        fallback = mode;
    }
    
    /**
     * Specify how many detailmaps to tile over the whole terrain
     */
    public void setTileScale(int scale){
        env.setTileScale(scale);
    }
    
    /**
     * Add a terrain model to this pass.
     * 
     * Identical to TerrainPass.add
     */
    public void addTerrain(Spatial terrain){
        add(terrain);
    }
    
    /**
     * Adds a detailmap, using the specified alphamap for blending.
     */
    public void addDetail(Texture detailmap, Texture alphamap, int tileScale){
        if (alphamap == null)
            base = new BaseLayer(detailmap);
        else{
            AlphaDetailLayer adl = new AlphaDetailLayer(detailmap,alphamap);
            if (tileScale != -1)
                adl.setScaleOverride(tileScale);
            
            detail.add(adl);
        }
    }
    
    public void addDetail(Texture detailmap, Texture alphamap){
        addDetail(detailmap,alphamap,-1);
    }
    
    /**
     * Same as addTile(Texture,Texture) but uses URLs instead
     */
    public void addDetail(URL detailmap, URL alphamap){
        addDetail(TextureManager.loadTexture(detailmap,MinificationFilter.Trilinear,MagnificationFilter.Bilinear,0f,false),
                alphamap == null ? null : TextureManager.loadTexture(alphamap,MinificationFilter.Trilinear,MagnificationFilter.Bilinear,0f,false),
                -1);
    }
    
    public void setLightmap(Texture lightmap, CombinerScale modulateScale){
        light = new LightLayer(lightmap, modulateScale);
        this.lightmap = true;
    }
    
    public void setLightmap(URL lightmap, CombinerScale modulateScale){
        setLightmap(TextureManager.loadTexture(lightmap,MinificationFilter.Trilinear,MagnificationFilter.Bilinear,0f,false), modulateScale);
    }
    
    public void setDynamicLighting(CombinerScale modulateScale){
        light = new LightLayer(modulateScale);
        lightmap = false;
    }
    
    public void setFog(FogState fs){
        fog = new FogLayer();
        setPassState(fs);
    }
    
    protected void copySpatialCoords(Spatial spat, int batch, int srcUnit, int targetUnit, int scale){
        if (spat instanceof Geometry)
            ((Geometry)spat).copyTextureCoordinates(srcUnit, targetUnit, scale);
        else if (spat instanceof Node){
            for (Spatial s: ((Node)spat).getChildren()){
                copySpatialCoords(s,batch,srcUnit,targetUnit,scale);
            }
        }
    }
    protected void copyPassCoords(int batch, int srcUnit, int targetUnit, int scale){
        for (Spatial spat: spatials){
            copySpatialCoords(spat,batch,srcUnit,targetUnit,scale);
        }
    }
    
    protected void generate(Renderer r){
        GLSLShaderObjectsState glsl = r.createGLSLShaderObjectsState();
        FragmentProgramState fp = r.createFragmentProgramState();
        
        if (GLSLShaderObjectsState.isSupported() && (fallback==MODE_BEST || fallback==MODE_GLSL)){
            renderMethod = 2;
        }else if (fp.isSupported() && (fallback==MODE_BEST || fallback==MODE_FRAG_PROGRAM)){
            renderMethod = 1;
        }else if (fallback==MODE_BEST || fallback==MODE_FIXED_FUNC){
            renderMethod = 0;
        }
        
        boolean lightAdded = false;
        if (!lightmap && light != null && renderMethod==0){
            env.addLayer(light);
            lightAdded = true;
        }
        
        env.addLayer(base);
        for (int i = 0; i < detail.size(); i++){
            env.addLayer(detail.get(i));
        }
        if (!lightAdded && light != null) env.addLayer(light);
        if (fog != null) env.addLayer(fog);
            
        if (renderMethod == MODE_GLSL){
            glsl = env.createGLSLShader(r);
            TextureState[] ts = env.createShaderPasses(r);

            copyPassCoords(0,0,1,env.getTileScale());
            
            RenderState[] pass = new RenderState[RenderState.RS_MAX_STATE];
            pass[RenderState.RS_TEXTURE] = ts[0];
            pass[RenderState.RS_GLSL_SHADER_OBJECTS] = glsl;
            passList.add(pass);
        }else if (renderMethod == MODE_FRAG_PROGRAM){
            copyPassCoords(0,0,1,env.getTileScale());

            TextureState[] ts = env.createShaderPasses(r);
            fp = env.createARBShader(r);

            RenderState[] pass = new RenderState[RenderState.RS_MAX_STATE];
            pass[RenderState.RS_TEXTURE] = ts[0];
            pass[RenderState.RS_FRAGMENT_PROGRAM] = fp;
            passList.add(pass);
        }else{
            for (int i = 0; i < TextureState.getNumberOfFixedUnits(); i++){
                copyPassCoords(0,0,i,1);
            }

            TextureState[] ts = env.createFixedFuncPasses(r);
            BlendState[] as = env.getFixedFuncAlphaStates();

            for (int i = 0; i < ts.length; i++){
                RenderState[] pass = new RenderState[RenderState.RS_MAX_STATE];
                pass[RenderState.RS_TEXTURE] = ts[i];
                pass[RenderState.RS_BLEND]   = as[i];
                passList.add(pass);
            }
        }
    }
    
    @Override
    protected void doUpdate(JmeContext cx){
        for (Spatial spat: spatials){
            spat.updateGeometricState(cx.getPassManager().getTPF(), true);
        }
    }
    
    protected void applyPassStates(RenderState[] passStates, RenderContext rc) {
        for (int x = RenderState.RS_MAX_STATE; --x >= 0; ) {
            if (passStates[x] != null) {
                savedStates[x] = rc.enforcedStateList[x];
                rc.enforcedStateList[x] = passStates[x];
            }
        }
    }
    
    protected void resetOldStates(RenderState[] passStates, RenderContext rc) {
        for (int x = RenderState.RS_MAX_STATE; --x >= 0; ) {
            if (passStates[x] != null) {
                rc.enforcedStateList[x] = savedStates[x];
            }
        }
    }
    
    @Override
    public void initPass(JmeContext cx){
        generate(cx.getRenderer());
    }
    
    @Override
    protected void doRender(JmeContext cx) {
        RenderContext rc = cx.getRenderContext();
        Renderer r = cx.getRenderer();
        
        for (int pass = 0; pass < passList.size(); pass++){
            RenderState[] rs = passList.get(pass);
            applyPassStates(rs, rc);
            for (int j = 0; j < spatials.size(); j++){
                r.draw(spatials.get(j));
            }
            resetOldStates(rs, rc);
        }
    }
    
    
}