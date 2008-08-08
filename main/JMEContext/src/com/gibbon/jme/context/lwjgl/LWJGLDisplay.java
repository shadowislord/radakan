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
import com.gibbon.jme.util.IJmeDisplay;
import com.jme.renderer.lwjgl.LWJGLRenderer;
import com.jme.system.GameSettings;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.lwjgl.LWJGLException;
import org.lwjgl.input.Keyboard;
import org.lwjgl.input.Mouse;
import org.lwjgl.opengl.*;

public class LWJGLDisplay extends Thread implements IJmeDisplay {
    
    static final Logger logger = Logger.getLogger("JmeContextLogger");
    
    private LWJGLContext context;
    private String title;
    
    private boolean fullscreen, vsync;
    
    private PixelFormat pf;
    private DisplayMode mode;
    
    private volatile boolean closing = false;
    private AtomicBoolean active = new AtomicBoolean(false);
    
    private boolean needRestart = false;
    
    private UncaughtExceptionHandler exHandler;
    
    protected DisplayMode getDisplayMode(GameSettings gs) throws LWJGLException{
        if (!gs.isFullscreen())
            return new DisplayMode(gs.getWidth(), gs.getHeight());
            
        DisplayMode m = null;
        for (DisplayMode dm: Display.getAvailableDisplayModes()){
            if (dm.getWidth()==gs.getWidth() 
             && dm.getHeight()==gs.getHeight() 
             && dm.getBitsPerPixel()==gs.getDepth()
             && dm.getFrequency()==gs.getFrequency()){
                m = dm;
            }
        }
        if (m==null)
            // Default mode
            return new DisplayMode(640,480);
        else
            return m;
    }
    
    protected LWJGLDisplay(LWJGLContext context, String title) {
        super(context.getName()+"-"+context.getIndex());
        this.context=context;
        this.title= title;
        
        GameSettings gs = context.getSettings();
        
        fullscreen = gs.isFullscreen();
        vsync = gs.isVerticalSync();

        try {
            mode = getDisplayMode(context.getSettings());
        } catch (LWJGLException ex) {
            logger.throwing("LWJGLDisplay", "<init>", ex);
        }
    }
    
    PixelFormat getPixelFormat(){
        return pf;
    }
    
    public static LWJGLDisplay create(LWJGLContext context, String title){
        return new LWJGLDisplay(context,title);
    }
    
    private Object startupLock = new Object();
    
    public void waitFor() throws InterruptedException{
        synchronized (startupLock){
            while (!isActive()){
                startupLock.wait();
            }
        }
    }
    
    @Override
    public void run(){
        try {
            if (exHandler==null)
                exHandler = new DefaultUncaughtExceptionHandler();
            
            setUncaughtExceptionHandler(exHandler);
            
            GameSettings gs = context.getSettings();
            //pf = new PixelFormat(gs.getAlphaBits(),
            //                                 gs.getDepthBits(),
            //                                 gs.getStencilBits(),
            //                                 gs.getSamples());
            
            pf = new PixelFormat(gs.getDepth(),
                            gs.getAlphaBits(),
                            gs.getDepthBits(),
                            gs.getStencilBits(),
                            gs.getSamples(), 0 /* no aux buffers */,
                            gs.getInt("GameAccumBPP", 0),
                            gs.getInt("GameAccumAlphaBits", 0),
                            gs.getBoolean("GameStereo", false),
                            gs.getBoolean("GameFloatingPointBuffer", false));
            
            Display.setTitle(title);
            Display.setFullscreen(fullscreen);
            Display.setDisplayMode(mode);
            
            LWJGLContext parent = (LWJGLContext) context.getParentContext();
            if (parent==null){
                Display.create(pf);
            }else{
                // use the parent pixel format otherwise we get errors
                
                // XXX: Might want to add some warning if the user specified
                // different pixel formats for the parent and child
                Display.create(parent.getPixelFormat(), parent.getDrawable());
            }
            Keyboard.poll();
            Mouse.poll();
            
            LWJGLContext.setThreadContext(context);
            context.setRenderer(new LWJGLRenderer(getWidth(),getHeight()));

            context.contextInit();
            
            synchronized (startupLock){
                active.set(true);
                startupLock.notifyAll();
            }
            
            LWJGLRenderer r = (LWJGLRenderer) context.getRenderer();
            int fps = gs.getFramerate();
            
            logger.log(Level.INFO,"Display rendering");
            
            while (isActive()){
                closing = Display.isCloseRequested();
                
                if (needRestart)
                    restart0();
                    
                r.clearBuffers();
                
                context.getTimer().update();
                context.getPassManager().run(context);
                
                r.displayBackBuffer();
                
                if (!active.get())
                    break;
                
                if (fps > 0)
                    Display.sync(fps);
            }
            
            context.contextDispose();
            Display.destroy();
            
            logger.log(Level.INFO,"Display destroyed");
        } catch (LWJGLException ex) {
            logger.throwing("LWJGLDisplay", "run", ex);
            ex.printStackTrace();
        }
    }
    
    public void setPosition(final int x, final int y){
        Display.setLocation(x,y);
    }
    
    void restart0(){
        GameSettings gs = context.getSettings();

        try {
            mode = getDisplayMode(gs);
            
            fullscreen = gs.isFullscreen();
            vsync = gs.isVerticalSync();
            context.contextRestart();
            
            Display.setDisplayMode(mode);
            Display.setFullscreen(fullscreen);
            
            needRestart = false;
            
            logger.log(Level.INFO,"Display restarted");
        } catch (LWJGLException ex) {
            logger.throwing("LWJGLDisplay", "restart", ex);
        }
    }
    
    public void restart(){
        needRestart = true;
    }
    
    void shutdown(){
        active.set(false);
        closing = true;
    }

    public boolean isActive() {
        return active.get();
    }

    public boolean isClosing() {
        return closing;
    }

    public int getWidth() {
        return mode.getWidth();
    }

    public int getHeight() {
        return mode.getHeight();
    }

    public int getBitsPerPixel() {
        return mode.getBitsPerPixel();
    }

    public int getFrequency() {
        return mode.getFrequency();
    }

    public boolean isFullscreen() {
        return fullscreen;
    }

    public boolean isVsync() {
        return vsync;
    }

}
class DefaultUncaughtExceptionHandler implements Thread.UncaughtExceptionHandler {
    
	public void uncaughtException(Thread t, Throwable e) {
            LWJGLDisplay.logger.throwing("DefaultUncaughtExceptionHandler","uncaughtException",e);
            e.printStackTrace();
            JmeContext.get().dispose();
	}
        
}
