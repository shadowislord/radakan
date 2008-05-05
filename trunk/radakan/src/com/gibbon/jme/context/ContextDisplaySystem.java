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
import com.jme.system.GameSettings;
import com.jme.system.PreferencesGameSettings;
import com.jmex.awt.JMECanvas;
import java.awt.Canvas;
import java.util.prefs.Preferences;
import org.lwjgl.opengl.RenderTexture;

/**
 * Dummy display system which allows original jME code to function properly with JmeContext.
 * 
 * @author MomokoFan
 */
public class ContextDisplaySystem extends DisplaySystem {
    
    public ContextDisplaySystem(){
        system = new ContextSystemProvider(this);
        created = true;
    }
    
    public String getAdapter() {
        return null;
    }

    public String getDriverVersion() {
        return null;
    }

    public String getDisplayVendor() {
        return null;
    }

    public String getDisplayRenderer() {
        return null;
    }

    public String getDisplayAPIVersion() {
        return null;
    }

    public boolean isValidDisplayMode(int width, int height, int bpp, int freq) {
        return true;
    }

    public void setVSyncEnabled(boolean enabled) {
    }

    public void setTitle(String title) {
    }

    public void createWindow(int w, int h, int bpp, int frq, boolean fs) {
    }

    public void createHeadlessWindow(int w, int h, int bpp) {
    }

    public Canvas createCanvas(int w, int h){
        return null;
    }

    public void recreateWindow(int w, int h, int bpp, int frq, boolean fs) {
    }

    public Renderer getRenderer() {
        if (JmeContext.get()==null)
            return null;
        
        return JmeContext.get().getRenderer();
    }

    public void setRenderer(Renderer r) {
    }

    public boolean isActive() {
        return JmeContext.get().isActive();
    }

    public boolean isClosing() {
        return false;
    }

    public void reset() {
    }

    public void close() {
        JmeContext.get().dispose();
    }

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

    protected void updateDisplayBGC() {
    }

    public void setIcon(Image[] iconImages) {
    }

    public void setCurrentCanvas(JMECanvas canvas) {
    }

    public RenderContext getCurrentContext() {
        return JmeContext.get().getRenderContext();
    }

    public void initForCanvas(int width, int height) {
    }

    public RenderContext removeContext(Object contextKey) {
        RenderContext context = JmeContext.get().getRenderContext();
        JmeContext.get().dispose();
        return context;
    }

    @Override
    public void moveWindowTo(int locX, int locY) {
        throw new UnsupportedOperationException("Not supported yet.");
    }
    

}
