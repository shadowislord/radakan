/*
 * Copyright (c) 2007, MFKARPG
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name of the Gibbon Entertainment nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY 'Gibbon Entertainment' "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL 'Gibbon Entertainment' BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package com.gibbon.jme.context;

import java.util.HashMap;
import java.util.logging.Logger;

import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.jme.image.Image;
import com.jme.renderer.RenderContext;
import com.jme.renderer.Renderer;
import com.jme.renderer.TextureRenderer;
import com.jme.renderer.TextureRenderer.Target;
import com.jme.renderer.lwjgl.LWJGLRenderer;
import com.jme.renderer.lwjgl.LWJGLTextureRenderer;
import com.jme.system.DisplaySystem;
import com.jme.system.canvas.JMECanvas;

/**
 * Dummy display system which allows original jME code to function properly with JmeContext.
 * 
 * @author MomokoFan
 */
public class ContextDisplaySystem extends DisplaySystem {
    
    private static final Logger logger = Logger.getLogger(ContextDisplaySystem.class.getName());
    
    /**
     * Do not call this method. Done automatically by JmeContext.
     */
    public ContextDisplaySystem(){
        //system = new ContextSystemProvider(this);
        created = true;
        logger.finer("jME displays bound to context system");
    }
    
    /**
     * @return null
     */
    public String getAdapter() {
        return null;
    }

    /**
     * @return null
     */
    public String getDriverVersion() {
        return null;
    }

    /**
     * @return null
     */
    public String getDisplayVendor() {
        return null;
    }

    /**
     * @return null
     */
    public String getDisplayRenderer() {
        return null;
    }

    /**
     * @return null
     */
    public String getDisplayAPIVersion() {
        return null;
    }

    /**
     * @return true
     */
    public boolean isValidDisplayMode(int width, int height, int bpp, int freq) {
        return true;
    }

    /**
     * Does absolutely nothing..
     */
    public void setVSyncEnabled(boolean enabled) {
    }

    /**
     * Does absolutely nothing..
     */
    public void setTitle(String title) {
    }

    /**
     * Does absolutely nothing..
     */
    public void createWindow(int w, int h, int bpp, int frq, boolean fs) {
    }

    /**
     * Does absolutely nothing..
     */
    public void createHeadlessWindow(int w, int h, int bpp) {
    }

    @Override
    public JMECanvas createCanvas(int w, int h, String type, HashMap<String, Object> props) {
            return null;
    }

    /**
     * Does absolutely nothing..
     */
    public void recreateWindow(int w, int h, int bpp, int frq, boolean fs) {
    }

    /**
     * @return The renderer of the currently bound JmeContext
     * @see JmeContext#get()
     */
    public Renderer getRenderer() {
        if (JmeContext.get()==null)
            return null;
        
        return JmeContext.get().getRenderer();
    }

    /**
     * Does absolutely nothing..
     */
    public void setRenderer(Renderer r) {
    }

    /**
     * @return true if the currently bound JmeContext is active
     */
    public boolean isActive() {
        return JmeContext.get().isActive();
    }

    /**
     * @return false
     */
    public boolean isClosing() {
        return false;
    }

    /**
     * Does absolutely nothing..
     */
    public void reset() {
    }

    /**
     * Closes the current bound JmeContext, but should actually do nothing..
     */
    public void close() {
        JmeContext.get().dispose();
    }

    /**
     * Creates a TextureRenderer object<br/>
     * Since this functionality is not supported by JmeContext, this method will do it.
     * 
     * @param width
     * @param height
     * @param target
     * @return The created texture renderer.
     */
    public TextureRenderer createTextureRenderer(int width, int height, Target target) {
        // XXX: LWJGL specific code here, JmeContext interface required for texturerenderer contextes.
        
        JmeContext cx = JmeContext.get();
        
        assert cx != null && cx instanceof LWJGLContext && cx.getRenderer() != null;
        
//        TextureRenderer textureRenderer = new LWJGLTextureRenderer( width, height, (LWJGLRenderer) getRenderer());
        TextureRenderer textureRenderer = new LWJGLTextureRenderer(width, height, null, (LWJGLRenderer) getRenderer());
        if (!textureRenderer.isSupported()) {
            textureRenderer = null;
            
            /* Might need later... probably not
            int lwjglTarget = 0;
            if ( target == Target.Texture1D ) {
                lwjglTarget = RenderTexture.RENDER_TEXTURE_1D;
            }
            else if ( target == Target.Texture2D ) {
                lwjglTarget = RenderTexture.RENDER_TEXTURE_2D;
            }
            else if ( target == Target.TextureCubeMap ) {
                lwjglTarget = RenderTexture.RENDER_TEXTURE_CUBE_MAP;
            }

            //boolean useRGB, boolean useRGBA, boolean useDepth, boolean isRectangle, int target, int mipmaps
           // RenderTexture renderTexture = new RenderTexture(false, true, true, false, lwjglTarget, 0);
            */
//            textureRenderer = new LWJGLPbufferTextureRenderer( width, height, (LWJGLRenderer) getRenderer(), renderTexture);
            textureRenderer =  DisplaySystem.getDisplaySystem().createTextureRenderer(width, height, target);
           
        }
        
        return textureRenderer;
    }

    /**
     * Does absolutely nothing..
     */
    protected void updateDisplayBGC() {
    }

    /**
     * Does absolutely nothing..
     */
    public void setIcon(Image[] iconImages) {
    }

    /**
     * Does absolutely nothing..
     */
    public void setCurrentCanvas(JMECanvas canvas) {
    }

    /**
     * Returns the rendercontext of the currently bound JmeContext
     */
    public RenderContext<?> getCurrentContext() {
        return JmeContext.get().getRenderContext();
    }

    /**
     * Does absolutely nothing..
     */
    public void initForCanvas(int width, int height) {
    }

    /**
     * ?
     * I don't know why it does what it does.
     * 
     * @param contextKey
     * @return the render context removed
     */
    public RenderContext<?> removeContext(Object contextKey) {
        RenderContext<?> context = JmeContext.get().getRenderContext();
        JmeContext.get().dispose();
        return context;
    }

    /**
     * New method in jME 2.0. Doesn't do anything.
     * 
     * @param locX
     * @param locY
     */
    @Override
    public void moveWindowTo(int locX, int locY) {
    }
    

}
