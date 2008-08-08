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

package com.gibbon.jme.context.lwjgl;

import com.gibbon.jme.util.IJmePixelBuffer;
import com.jme.image.Image;
import com.jme.renderer.ColorRGBA;
import com.jme.renderer.lwjgl.LWJGLRenderer;
import com.jme.system.GameSettings;
import com.jme.system.JmeException;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.logging.Logger;

import org.lwjgl.LWJGLException;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.Pbuffer;
import org.lwjgl.opengl.PixelFormat;

public class LWJGLPixelBuffer implements IJmePixelBuffer {
    
    static final Logger logger = Logger.getLogger("JmeContextLogger");
    
    private Pbuffer pb;
    private LWJGLContext context;
    private LWJGLContext previous;
    
    private PixelFormat pf;
    private volatile boolean active = false;
    
    public LWJGLPixelBuffer(LWJGLContext cx) {
        context = cx;
    }
    
    public static LWJGLPixelBuffer create(LWJGLContext context){
        return new LWJGLPixelBuffer(context);
    }
    
    public void start(){
        if (!context.isPbufferSupported())
            throw new JmeException("Pbuffer is not supported!");
        
        
        GameSettings gs = context.getSettings();
        LWJGLContext parent = (LWJGLContext) context.getParentContext();
        
        try {
            if (parent != null){
                pb = new Pbuffer(gs.getWidth(),gs.getHeight(), parent.getPixelFormat(), parent.getDrawable());
            }else{
                pf = new PixelFormat(gs.getAlphaBits(),gs.getDepthBits(),gs.getStencilBits(),gs.getSamples());
                pb = new Pbuffer(gs.getWidth(),gs.getHeight(),pf,null);
            }
            
            // enter the context so that renderer can initialize
            enterContext();
            context.setRenderer(new LWJGLRenderer(gs.getWidth(),gs.getHeight()));
            context.getRenderer().setHeadless(true);
            context.getRenderer().setBackgroundColor(ColorRGBA.black);
            context.contextInit();
            exitContext();
        } catch (LWJGLException ex) {
            ex.printStackTrace();
            logger.throwing("LWJGLPixelBuffer", "run", ex);
        }
    }
    
    public void enterContext(){
        previous = (LWJGLContext) LWJGLContext.getThreadContext();
        LWJGLContext.setThreadContext(context);
    }
    
    public void exitContext(){
        LWJGLContext.setThreadContext(previous);
        previous = null;
    }

    Pbuffer getPbuffer(){
        return pb;
    }
    
    PixelFormat getPixelFormat(){
        return pf;
    }
    
    public boolean isActive() {
        return active;
    }

    public int getWidth() {
        return pb.getWidth();
    }

    public int getHeight() {
        return pb.getHeight();
    }

    public int getBitsPerPixel() {
        return pf.getBitsPerPixel();
    }

    public void shutdown() {
        context.contextDispose();
        pb.destroy();
        active = false;
    }

    public ByteBuffer getPixels(ByteBuffer store, int x, int y, int w, int h) {
        if (store==null) store = ByteBuffer.allocateDirect(3*w*h);
        GL11.glReadPixels(x, y, w, h, GL11.GL_RGB, GL11.GL_UNSIGNED_BYTE, store);
        return store;
    }
    
    public IntBuffer getPixels(IntBuffer store, int x, int y, int w, int h) {
        if (store==null) store = ByteBuffer.allocateDirect(4*w*h).asIntBuffer();
        GL11.glReadPixels(x, y, w, h, GL11.GL_RGBA, GL11.GL_UNSIGNED_BYTE, store);
        return store;
    }
    
    /*
    private static final int[] imageFormats = { GL11.GL_RGBA, 
                                                GL11.GL_RGB,
                                                GL11.GL_RGBA,
                                                GL11.GL_RGBA, 
                                                GL11.GL_LUMINANCE_ALPHA };
    
    private static final int[] bitSizes = { 4,
                                            3,
                                            4,
                                            4,
                                            2 };*/
    

    public void getPixels(Image store, int x, int y){
//        GL11.glReadPixels(x,y,
//                          store.getWidth(),
//                          store.getHeight(),
//                          imageFormats[store.getType()],
//                          GL11.GL_UNSIGNED_BYTE,
//                          store.getData());
    }

    public Image getPixels(int x, int y, int w, int h, int imageType) {
        throw new UnsupportedOperationException("Not supported yet.");
    }
    
//    public Image getPixels(int x, int y, int w, int h, int imageType){
//        Image img = new Image(imageType, w, h, null);
//        img.setData(ByteBuffer.allocateDirect(w*h*bitSizes[imageType]));
//        getPixels(img,x,y);
//        return img;
//    }
}
