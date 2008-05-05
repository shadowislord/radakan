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

import com.jme.renderer.ColorRGBA;
import com.jme.renderer.lwjgl.LWJGLRenderer;
import com.jme.system.GameSettings;
import com.jme.system.JmeException;
import java.awt.GraphicsEnvironment;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.lwjgl.LWJGLException;
import org.lwjgl.opengl.AWTGLCanvas;
import org.lwjgl.opengl.Drawable;
import org.lwjgl.opengl.PixelFormat;

public class LWJGLCanvas extends AWTGLCanvas {
    
    static final Logger logger = Logger.getLogger("JmeContextLogger");
    
    private LWJGLContext context;
    private boolean vsync;
    private PixelFormat pf;
    private volatile boolean active = false;
    private boolean needRestart = false;
    
    private Object startupLock = new Object();
    
    public LWJGLCanvas(LWJGLContext cx, PixelFormat pf, Drawable shared_drawable) throws LWJGLException {
        super(GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice(), pf, shared_drawable);
        context = cx;
        this.pf = pf;
        
        addComponentListener(new ComponentAdapter() {
            public void componentResized(ComponentEvent ce) {
                needRestart = true;
            }
        });
        
        logger.info("Canvas created");
    }
    
    public static LWJGLCanvas create(LWJGLContext context){
        GameSettings gs = context.getSettings();
        PixelFormat pf = new PixelFormat(gs.getAlphaBits(),gs.getDepthBits(),gs.getStencilBits(),gs.getSamples());
        LWJGLContext parent = (LWJGLContext) context.getParentContext();
        
        try {
            if (parent != null){
                return new LWJGLCanvas(context, parent.getPixelFormat(), parent.getDrawable());
            }else{
                return new LWJGLCanvas(context, pf, null);
            }
        } catch (LWJGLException ex) {
            logger.throwing("LWJGLCanvas", "create", ex);
        }
        return null;
    }
    
    public void start(){
    }
    
    public void restart(){
        needRestart = true;
    }
    
    void shutdown(){
        active = false;
    }
    
    public void waitFor() throws InterruptedException{
        synchronized (startupLock){
            while (!active){
                startupLock.wait();
            }
        }
    }
    
    public void run(){
        setVSyncEnabled(context.getSettings().isVerticalSync());
        
        context.setRenderer(new LWJGLRenderer(640,480));
        context.getRenderer().setHeadless(true);
        context.getRenderer().setBackgroundColor(ColorRGBA.black);
        context.contextInit();
        synchronized (startupLock){
            active = true;
            startupLock.notifyAll();
        }
        // main loop is handled in paintGL (defined by AWTGLCanvas in LWJGL)
    }

    public void initGL(){
        run();
        context.getRenderer().reinit(getWidth(), getHeight());
    }
    
    public void paintGL(){
        // getContext returns the LWJGL's library context, not Jme's
        // If it's null then the canvas has been removed from the frame
        if (getContext()==null)
            active = false;
        
        // User ordered disposal, dispose context resources from the context thread
        if (!active && context!=null){
            context.contextDispose();
            logger.info("Canvas disposed");
            return;
        }else if (!active)
            return;
        
        if (needRestart)
            context.contextRestart();
        
        context.setThreadContext(context);
        
        LWJGLRenderer r = (LWJGLRenderer) context.getRenderer();
        
        r.clearBuffers();

        context.getTimer().update();
        context.getPassManager().run(context);
        //r.displayBackBuffer();
        
        try {
            swapBuffers();
        } catch (LWJGLException ex) {
            logger.throwing("LWJGLCanvas", "paintGL", ex);
        }

        if (isVisible()){
            repaint();
        }
        
        context.setThreadContext(null);
    }
    
    public boolean isActive(){
        return active;
    }

    PixelFormat getPixelFormat() {
        return pf;
    }
}
