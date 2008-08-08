/*
 * Copyright (c) 2003-2007 jMonkeyEngine
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of 'jMonkeyEngine' nor the names of its contributors 
 *   may be used to endorse or promote products derived from this software 
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
package com.gibbon.jme.context;

import com.jme.image.Texture;
import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.WrapMode;
import com.jme.image.Texture2D;
import com.jme.renderer.Camera;
import com.jme.renderer.ColorRGBA;
import com.jme.renderer.Renderer;
import com.jme.renderer.TextureRenderer;
import com.jme.renderer.TextureRenderer.Target;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.Spatial.CullHint;
import com.jme.scene.Spatial.LightCombineMode;
import com.jme.scene.Spatial.TextureCombineMode;
import com.jme.scene.shape.Quad;
import com.jme.scene.state.BlendState;
import com.jme.scene.state.BlendState.DestinationFunction;
import com.jme.scene.state.BlendState.SourceFunction;
import com.jme.scene.state.GLSLShaderObjectsState;
import com.jme.scene.state.LightState;
import com.jme.scene.state.RenderState;
import com.jme.scene.state.TextureState;
import com.jme.system.DisplaySystem;
import com.jme.util.resource.ResourceLocatorTool;
import java.net.URL;

/**
 * GLSL bloom effect pass. - Render supplied source to a texture - Extract
 * intensity - Blur intensity - Blend with first pass
 * 
 * @author Rikard Herlitz (MrCoder) - initial implementation
 * @author Joshua Slack - Enhancements and reworking to use a single
 *         texrenderer, ability to reuse existing back buffer, faster blur,
 *         throttling speed-up, etc.
 * @author Momoko_Fan - Ported to JmeContext system
 */
public class CXBloomPass extends RenderPass {

    private int renderScale;
    private Camera cam;
    
    private float throttle = 1 / 50f;
    private float sinceLast = 1;
    private TextureRenderer tRenderer;
    private Texture2D mainTexture;
    private Texture2D secondTexture;
    private Texture2D screenTexture;
    private Quad fullScreenQuad;
    private GLSLShaderObjectsState extractionShader;
    private GLSLShaderObjectsState blurShader;
    private GLSLShaderObjectsState finalShader;
    private int nrBlurPasses;
    private float blurSize;
    private float blurIntensityMultiplier;
    private float exposurePow;
    private float exposureCutoff;
    private boolean supported = true;
    private boolean useCurrentScene = false;

    /**
     * Reset bloom parameters to default
     */
    public void resetParameters() {
        nrBlurPasses = 2;
        blurSize = 0.02f;
        blurIntensityMultiplier = 1.3f;
        exposurePow = 3.0f;
        exposureCutoff = 0.0f;
    }

    /**
     * Release pbuffers in TextureRenderer's. Preferably called from user cleanup method.
     */
    public void cleanup() {
        //super.cleanUp();
        if (tRenderer != null) {
            tRenderer.cleanup();
        }
    }

    public boolean isSupported() {
        return supported;
    }

    /**
     * Creates a new bloom renderpass
     *
     * @param cam		 Camera used for rendering the bloomsource
     * @param renderScale Scale of bloom texture
     */
    public CXBloomPass(Camera cam, int renderScale) {
        super(PassType.POST_RENDER, "bloom");
        this.cam = cam;
        this.renderScale = renderScale;
        resetParameters();
    }

    /**
     * Initializes RTT, textures and models.<br/>
     * Only used internally, do not call.
     * 
     * @param cx
     */
    @Override
    public void initPass(JmeContext cx){
        super.initPass(cx);

        DisplaySystem display = DisplaySystem.getDisplaySystem();
        Renderer r = cx.getRenderer();
        
        //Create texture renderers and rendertextures(alternating between two not to overwrite pbuffers)
        tRenderer = display.createTextureRenderer(
                r.getWidth() / renderScale,
                r.getHeight() / renderScale,
                Target.Texture2D);
        
        if (!tRenderer.isSupported()) {
            supported = false;
            return;
        }
        
        tRenderer.setMultipleTargets(true);
        tRenderer.setBackgroundColor(new ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f));
        tRenderer.setCamera(cam);

        mainTexture = new Texture2D();
        mainTexture.setWrap(WrapMode.Clamp);
        mainTexture.setMagnificationFilter(MagnificationFilter.Bilinear);
        tRenderer.setupTexture(mainTexture);

        secondTexture = new Texture2D();
        secondTexture.setWrap(WrapMode.Clamp);
        secondTexture.setMagnificationFilter(MagnificationFilter.Bilinear);
        tRenderer.setupTexture(secondTexture);

        screenTexture = new Texture2D();
        screenTexture.setWrap(WrapMode.Clamp);
        screenTexture.setMagnificationFilter(MagnificationFilter.Bilinear);
        tRenderer.setupTexture(screenTexture);

        //Create extract intensity shader
        extractionShader = r.createGLSLShaderObjectsState();
        if (!extractionShader.isSupported()) {
            supported = false;
            return;
        }

        URL quadV = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_SHADER, "bloom_extract.vert");
        
        //URL extractV = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_SHADER, "bloom_extract.vert");
        URL extractF = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_SHADER, "bloom_extract.frag");
        extractionShader.load(quadV,extractF);
        extractionShader.setUniform("RT", 0);
        
        //URL blurV = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_SHADER, "bloom_blur.vert");
        URL blurF = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_SHADER, "bloom_blur.frag");
        blurShader = r.createGLSLShaderObjectsState();
        blurShader.load(quadV, blurF);
        blurShader.setUniform("RT", 0);
        
        //URL finalV = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_SHADER, "bloom_final.vert");
        URL finalF = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_SHADER, "bloom_final.frag");
        finalShader = r.createGLSLShaderObjectsState();
        finalShader.load(quadV, finalF);
        
        //Create fullscreen quad
        fullScreenQuad = new Quad("FullScreenQuad", display.getWidth() / 4, display.getHeight() / 4);
        fullScreenQuad.getLocalRotation().set(0, 0, 0, 1);
        fullScreenQuad.getLocalTranslation().set(display.getWidth() / 2, display.getHeight() / 2, 0);
        fullScreenQuad.getLocalScale().set(1, 1, 1);
        //fullScreenQuad.setRenderQueueMode(Renderer.QUEUE_ORTHO);

        fullScreenQuad.setCullHint(CullHint.Never);
        fullScreenQuad.setTextureCombineMode(TextureCombineMode.Replace);
        fullScreenQuad.setLightCombineMode(LightCombineMode.Off);

        TextureState ts = display.getRenderer().createTextureState();
        fullScreenQuad.setRenderState(ts);

        BlendState as = display.getRenderer().createBlendState();
        as.setBlendEnabled(true);
        as.setSourceFunction(SourceFunction.One);
        as.setDestinationFunction(DestinationFunction.One);
        fullScreenQuad.setRenderState(as);

        fullScreenQuad.updateRenderState();
        fullScreenQuad.updateGeometricState(0.0f, true);
    }
    
    /**
     * Helper class to get all spatials rendered in one TextureRenderer.render() call.
     */
    private class SpatialsRenderNode extends Node {

        private static final long serialVersionUID = 7367501683137581101L;

        @Override
        public void draw(Renderer r) {
            Spatial child;
            for (int i = 0,  cSize = spatials.size(); i < cSize; i++) {
                child = spatials.get(i);
                if (child != null) {
                    child.onDraw(r);
                }
            }
        }

        @Override
        public void onDraw(Renderer r) {
            draw(r);
        }
    }
    private final SpatialsRenderNode spatialsRenderNode = new SpatialsRenderNode();

    /**
     * Update the throttle value and other misc. <br/>
     * Only used internally, do not call this method.
     * 
     * @param cx
     */
    @Override
    protected void doUpdate(JmeContext cx) {
        super.doUpdate(cx);
        sinceLast += cx.getPassManager().getTPF();
    }

    /**
     * Renders this pass. <br/>
     * Only used internally, do not call this method.
     * 
     * @param cx
     */
    @Override
    public void doRender(JmeContext cx) {
        Renderer r = cx.getRenderer();
        
        if (!useCurrentScene && spatials.size() == 0) {
            return;
        }

        BlendState as = (BlendState) fullScreenQuad.states[RenderState.RS_BLEND];

        if (sinceLast > throttle) {
            sinceLast = 0;

            as.setEnabled(false);
            TextureState ts = (TextureState) fullScreenQuad.states[RenderState.RS_TEXTURE];

            // see if we should use the current scene to bloom, or only things added to the pass.
            if (useCurrentScene) {
                // grab backbuffer to texture
                tRenderer.copyToTexture(screenTexture,
                        DisplaySystem.getDisplaySystem().getWidth(),
                        DisplaySystem.getDisplaySystem().getHeight());
                ts.setTexture(screenTexture, 0);
            } else {
                //Render scene to texture
                tRenderer.render(spatialsRenderNode, mainTexture);
                ts.setTexture(mainTexture, 0);
            }
            
            //Extract intensity
            extractionShader.setUniform("exposurePow", getExposurePow());
            extractionShader.setUniform("exposureCutoff", getExposureCutoff());

            fullScreenQuad.states[RenderState.RS_GLSL_SHADER_OBJECTS] = extractionShader;
            tRenderer.render(fullScreenQuad, secondTexture);

            //Blur
            blurShader.setUniform("sampleDist0", getBlurSize());
            blurShader.setUniform("blurIntensityMultiplier", getBlurIntensityMultiplier());

            ts.setTexture(secondTexture, 0);
            fullScreenQuad.states[RenderState.RS_GLSL_SHADER_OBJECTS] = blurShader;
            tRenderer.render(fullScreenQuad, mainTexture);

            //Extra blur passes
            for (int i = 1; i < getNrBlurPasses(); i++) {
                if (i % 2 == 1) {
                    ts.setTexture(mainTexture, 0);
                    tRenderer.render(fullScreenQuad, secondTexture);
                } else {
                    ts.setTexture(secondTexture, 0);
                    tRenderer.render(fullScreenQuad, mainTexture);
                }
            }
            if (getNrBlurPasses() % 2 == 1) {
                ts.setTexture(mainTexture, 0);
            } else {
                ts.setTexture(secondTexture, 0);
            }
        }

        //Final blend
        as.setEnabled(false);

        //fullScreenQuadBatch.states[RenderState.RS_GLSL_SHADER_OBJECTS] = null;
        fullScreenQuad.states[RenderState.RS_GLSL_SHADER_OBJECTS] = finalShader;
        
        r.draw(fullScreenQuad);
    }

    /**
     * @return The throttle amount - or in other words, how much time in
     *         seconds must pass before the bloom effect is updated.
     */
    public float getThrottle() {
        return throttle;
    }

    /**
     * @param throttle
     *            The throttle amount - or in other words, how much time in
     *            seconds must pass before the bloom effect is updated.
     */
    public void setThrottle(float throttle) {
        this.throttle = throttle;
    }

    public float getBlurSize() {
        return blurSize;
    }

    public void setBlurSize(float blurSize) {
        this.blurSize = blurSize;
    }

    public float getExposurePow() {
        return exposurePow;
    }

    public void setExposurePow(float exposurePow) {
        this.exposurePow = exposurePow;
    }

    /**
     * @return The minimum exposure required in a pixel for it to be 'bloomed'.
     */
    public float getExposureCutoff() {
        return exposureCutoff;
    }

    /**
     * @param exposureCutoff The minimum exposure required in a pixel for it to be 'bloomed'.
     */
    public void setExposureCutoff(float exposureCutoff) {
        this.exposureCutoff = exposureCutoff;
    }

    public float getBlurIntensityMultiplier() {
        return blurIntensityMultiplier;
    }

    public void setBlurIntensityMultiplier(float blurIntensityMultiplier) {
        this.blurIntensityMultiplier = blurIntensityMultiplier;
    }
    
    /**
     * @return The number of blur passes
     */
    public int getNrBlurPasses() {
        return nrBlurPasses;
    }

    /**
     * Set the number of blur passes.<br/>
     * Higher number increases the bloom blurring effect, but reduces performance drastically.
     * 
     * @param nrBlurPasses
     */
    public void setNrBlurPasses(int nrBlurPasses) {
        this.nrBlurPasses = nrBlurPasses;
    }

    /**
     * @return True if the already rendered scene is used for blooming.
     */
    public boolean useCurrentScene() {
        return useCurrentScene;
    }

    /**
     * @param useCurrentScene Causes the bloom pass to bloom the scene already rendered to the screen
     */
    public void setUseCurrentScene(boolean useCurrentScene) {
        this.useCurrentScene = useCurrentScene;
    }
}
