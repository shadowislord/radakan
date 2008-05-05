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

package com.gibbon.jme.context;

import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.jme.image.Image;
import com.jme.renderer.RenderContext;
import com.jme.renderer.Renderer;
import com.jme.renderer.TextureRenderer;
import com.jme.renderer.TextureRenderer.Target;
import com.jme.renderer.lwjgl.LWJGLPbufferTextureRenderer;
import com.jme.renderer.lwjgl.LWJGLRenderer;
import com.jme.renderer.lwjgl.LWJGLTextureRenderer;
import com.jme.system.DisplaySystem;
import com.jmex.awt.JMECanvas;
import java.awt.Canvas;
import org.lwjgl.opengl.RenderTexture;

/**
 * Dummy display system which allows original jME code to function properly with JmeContext.
 * 
 * @author MomokoFan
 */
public class ContextDisplaySystem extends DisplaySystem {
    
    /**
     * Do not call this method. Done automatically by JmeContext.
     */
    public ContextDisplaySystem(){
        system = new ContextSystemProvider(this);
        created = true;
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

    /**
     * @param w
     * @param h
     * @return null
     */
    public Canvas createCanvas(int w, int h){
        return null;
    }

    /**
     * Does absolutely nothing..
     */
    public void recreateWindow(int w, int h, int bpp, int frq, boolean fs) {
    }

    /**
     * @return The renderer of the currently bound JmeContext
     * @see JmeContext.get()
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
     * @return
     */
    public TextureRenderer createTextureRenderer(int width, int height, Target target) {
        // XXX: LWJGL specific code here, JmeContext interface required for texturerenderer contextes.
        
        JmeContext cx = JmeContext.get();
        
        assert cx != null && cx instanceof LWJGLContext && cx.getRenderer() != null;
        
        TextureRenderer textureRenderer = new LWJGLTextureRenderer( width, height, (LWJGLRenderer) getRenderer());

        if (!textureRenderer.isSupported()) {
            textureRenderer = null;

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
            RenderTexture renderTexture = new RenderTexture(false, true, true, false, lwjglTarget, 0);
            
            textureRenderer = new LWJGLPbufferTextureRenderer( width, height, 
                    (LWJGLRenderer) getRenderer(), renderTexture);
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
    public RenderContext getCurrentContext() {
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
     * @return
     */
    public RenderContext removeContext(Object contextKey) {
        RenderContext context = JmeContext.get().getRenderContext();
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
