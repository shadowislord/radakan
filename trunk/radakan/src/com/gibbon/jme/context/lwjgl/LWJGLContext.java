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

import com.gibbon.jme.context.*;
import com.jme.input.joystick.JoystickInput;
import com.jme.renderer.Camera;
import com.jme.renderer.ColorRGBA;
import com.jme.renderer.RenderContext;
import com.jme.renderer.Renderer;
import com.jme.renderer.lwjgl.LWJGLRenderer;
import com.jme.scene.state.RenderState;
import com.jme.system.JmeException;
import com.jme.util.TextureManager;
import com.jme.util.Timer;
import com.jme.util.lwjgl.LWJGLTimer;
import java.awt.Canvas;
import org.lwjgl.LWJGLException;
import org.lwjgl.opengl.ARBMultisample;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.Drawable;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.Pbuffer;
import org.lwjgl.opengl.PixelFormat;

public class LWJGLContext extends JmeContext {
    
    private static boolean inited = false;
    RenderState[] defaultStates = new RenderState[RenderState.RS_MAX_STATE];
    
    private LWJGLContext parent = null;
    
    private LWJGLDisplay window = null;
    private LWJGLCanvas canvas = null;
    private LWJGLPixelBuffer pb = null;
    
    private static LWJGLTimer timer = null;
    
    public String getName() {
        return "LWJGL";
    }

    public boolean isActive() {
        if (type==CONTEXT_WINDOW && window!=null)
            return window.isActive();
        else if ( (type==CONTEXT_CANVAS) && canvas!=null)
            return canvas.isActive();
        else if ( type==CONTEXT_PBUFFER && pb != null )
            return pb.isActive();
        
        return false;
    }
    
    /**
     * NOTE: User thread call
     */
    public void start(){
        if (!inited)
            init();

        if (type==CONTEXT_WINDOW){
            window = LWJGLDisplay.create(this,settings.get("title","OpenGL Window"));
            window.start();
        }else if (type==CONTEXT_CANVAS){
            canvas = LWJGLCanvas.create(this);
            canvas.start();
        }else if (type==CONTEXT_PBUFFER){
            pb = LWJGLPixelBuffer.create(this);
            pb.start();
        }
    }
    
    /**
     * Waits for until the context is ready for drawing
     *
     * NOTE: User thread call
     */
    public void waitFor() throws InterruptedException{
        if (type==CONTEXT_WINDOW) window.waitFor();
        else if (type==CONTEXT_CANVAS) canvas.waitFor();
    }
    
    public void setParentContext(JmeContext parent) {
        if (!(parent instanceof LWJGLContext))
            throw new IllegalArgumentException("Specified parent context cannot be shared");
        
        if (isActive())
            throw new IllegalStateException("Context has been created already");
        
        this.parent = (LWJGLContext) parent;
    }
    
    public JmeContext getParentContext(){
        return parent;
    }
    
    public JmeDisplay getDisplay() {
        return window;
    }

    public Canvas getCanvas() {
        return canvas;
    }
    
    public JmePixelBuffer getPbuffer(){
        return pb;
    }
    
    public boolean isPbufferSupported(){
        return (Pbuffer.getCapabilities() & Pbuffer.PBUFFER_SUPPORTED) == Pbuffer.PBUFFER_SUPPORTED;
    }
    
    public Timer getTimer() {
        return timer;
    }
    
    public void restart() {
        if (isActive() && type==CONTEXT_WINDOW)
            window.restart();
        
        if (isActive() && type==CONTEXT_CANVAS)
            canvas.restart();
    }
    
    /**
     * Sets the current thread's context ThreadLocal to a value and makes the context current
     *
     * NOTE: Context thread call
     */
    static void setThreadContext(JmeContext context){
        if (context==null || getThreadContext()==context)
            return;
        
        LWJGLContext lc = (LWJGLContext) context;
        try{
            switch (lc.getType()){
                case CONTEXT_WINDOW:
                    Display.makeCurrent();
                    break;
                case CONTEXT_CANVAS:
                    lc.canvas.makeCurrent();
                    break;
                case CONTEXT_PBUFFER:
                    lc.pb.getPbuffer().makeCurrent();
                    break;
            }
            
            contextLocal.set(context);
        } catch (LWJGLException ex){
            throw new JmeException("Failed to make context current",ex);
        }
    }
    
    static JmeContext getThreadContext(){
        return contextLocal.get();
    }
    
    void setRenderer(LWJGLRenderer r){ 
        renderer = r; 
    }
    
    void init() {
        new ContextDisplaySystem();
        timer = new LWJGLTimer();
        Timer.setTimer(timer);
        inited = true;
    }

    public void dispose() {
        if (type==CONTEXT_WINDOW){
            window.shutdown();
        }else if (type==CONTEXT_CANVAS){
            canvas.shutdown();
        }else if (type==CONTEXT_PBUFFER){
            pb.shutdown();
        }
        // display & canvas will call contextDispose from their threads
    }
    
    /**
     * Dispose context thread specific resources
     *
     * NOTE: Context thread call
     */
    void contextDispose(){
        remove();
        setThreadContext(null);
        manager.cleanup(this);
        TextureManager.doTextureCleanup();
        renderer.cleanup();
    }

    /**
     * Initializes context thread specific resources
     * 
     * NOTE: Context thread call
     */
    void contextInit(){
        for (int i = 0; i < RenderState.RS_MAX_STATE-1; i++) {
            defaultStates[i] = renderer.createState(i);
            defaultStates[i].setEnabled(false);
            Renderer.defaultStateList[i] = defaultStates[i];
        }
        rc = new RenderContext(null);
        rc.setupRecords(renderer);
        
        Camera cam = null;
        if (type==CONTEXT_CANVAS){
            cam = renderer.createCamera(640,480);
            cam.setFrustumPerspective(45.0f,640/480,1,1000);
        }else if (type==CONTEXT_WINDOW){
            cam = renderer.createCamera(window.getWidth(),window.getWidth());
            cam.setFrustumPerspective(45.0f,window.getWidth()/window.getHeight(),1,1000);
        }else if (type==CONTEXT_PBUFFER){
            cam = renderer.createCamera(pb.getWidth(),pb.getWidth());
            cam.setFrustumPerspective(45.0f,pb.getWidth()/pb.getHeight(),1,1000);
        }
        
        cam.update();
        renderer.setCamera(cam);
        renderer.setBackgroundColor(ColorRGBA.black);
        
        if (settings.getSamples() > 0){
            GL11.glEnable(ARBMultisample.GL_MULTISAMPLE_ARB);
        }
        
        JoystickInput.get();
    }
    
    /**
     * NOTE: Context thread call
     */
    void contextRestart(){
        if (type==CONTEXT_WINDOW)
            renderer.reinit(window.getWidth(),window.getHeight());
        else if (type==CONTEXT_CANVAS)
            renderer.reinit(canvas.getWidth(),canvas.getHeight());
    }
    
    Drawable getDrawable(){
       if (type==CONTEXT_WINDOW)
           return Display.getDrawable();
       else if (type==CONTEXT_CANVAS)
           return canvas;
       else if (type==CONTEXT_PBUFFER)
           return pb.getPbuffer();
       
       return null;
    }
    
    PixelFormat getPixelFormat(){
        if (type==CONTEXT_WINDOW)
            return window.getPixelFormat();
        else if (type==CONTEXT_CANVAS)
            return canvas.getPixelFormat();
        else if (type==CONTEXT_PBUFFER)
            return pb.getPixelFormat();
        
        return null;
    }
    
    void setCanvasDrawing(boolean drawing){
        canvasDrawing.set(drawing);
    }
    
}
