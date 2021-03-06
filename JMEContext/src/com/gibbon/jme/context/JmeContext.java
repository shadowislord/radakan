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

import java.awt.Canvas;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.logging.Logger;
import java.util.prefs.Preferences;

import com.gibbon.jme.pass.PassManager;
import com.gibbon.jme.util.IJmePixelBuffer;
import com.gibbon.jme.util.IJmeDisplay;
import com.jme.renderer.RenderContext;
import com.jme.renderer.Renderer;
import com.jme.system.GameSettings;
import com.jme.system.PreferencesGameSettings;
import com.jme.system.PropertiesGameSettings;
import com.jme.util.Timer;

/**
 * The main class in the Jme context framework
 * <p>
 * Step by step usage:<br/>
 *   Call JmeContext.create() to create a new context
 *   Access the default context with JmeContext.get() or access by index with the other get method.
 *   Set the default context with JmeContext.setDefault()
 *   <p>
 *   Call dispose() to destroy a context, if the context is a canvas,
 *   it should be detached from the frame or applet it is on after disposal.
 *   <p>
 *   Accessing the display or canvas is possible with getDisplay & getCanvas respectively
 */
public abstract class JmeContext {
    
    public static final int CONTEXT_NONE            = 0, 
                            CONTEXT_WINDOW          = 1, 
                            CONTEXT_CANVAS          = 2,
                            CONTEXT_PBUFFER         = 4;
    
    protected static final Logger logger = Logger.getLogger("JmeContextLogger");
    protected static final GameSettings defaults;
    
    static {
    	//defaults = new PropertiesGameSettings("properties.cfg");
        //defaults = new PreferencesGameSettings(Preferences.userNodeForPackage(JmeContext.class), true, null);
        defaults = new GameSettings(){
			public void clear() throws IOException {
			}

			public String get(String name, String defaultValue) {
				return null;
			}

			public int getAlphaBits() {
				return 0;
			}

			public boolean getBoolean(String name, boolean defaultValue) {
				return false;
			}

			public byte[] getByteArray(String name, byte[] bytes) {
				return null;
			}

			public String getDefaultSettingsWidgetImage() {
				return null;
			}

			public int getDepth() {
				return 32;
			}

			public int getDepthBits() {
				return 8;
			}

			public double getDouble(String name, double defaultValue) {
				return 0;
			}

			public float getFloat(String name, float defaultValue) {
				return 0;
			}

			public int getFramerate() {
				// TODO Auto-generated method stub
				return 60;
			}

			public int getFrequency() {
				// TODO Auto-generated method stub
				return 60;
			}

			public int getHeight() {
				// TODO Auto-generated method stub
				return 480;
			}

			public int getInt(String name, int defaultValue) {
				// TODO Auto-generated method stub
				return 0;
			}

			public long getLong(String name, long defaultValue) {
				// TODO Auto-generated method stub
				return 0;
			}

			public Object getObject(String name, Object obj) {
				// TODO Auto-generated method stub
				return null;
			}

			public String getRenderer() {
				// TODO Auto-generated method stub
				return "LWJGL";
			}

			public int getSamples() {
				// TODO Auto-generated method stub
				return 0;
			}

			public int getStencilBits() {
				// TODO Auto-generated method stub
				return 0;
			}

			public int getWidth() {
				// TODO Auto-generated method stub
				return 640;
			}

			public boolean isFullscreen() {
				// TODO Auto-generated method stub
				return false;
			}

			public boolean isMusic() {
				// TODO Auto-generated method stub
				return false;
			}

			public boolean isNew() {
				// TODO Auto-generated method stub
				return false;
			}

			public boolean isSFX() {
				// TODO Auto-generated method stub
				return false;
			}

			public boolean isVerticalSync() {
				// TODO Auto-generated method stub
				return false;
			}

			public void save() throws IOException {
				// TODO Auto-generated method stub
				
			}

			public void set(String name, String value) {
				// TODO Auto-generated method stub
				
			}

			public void setAlphaBits(int alphaBits) {
				// TODO Auto-generated method stub
				
			}

			public void setBoolean(String name, boolean value) {
				// TODO Auto-generated method stub
				
			}

			public void setByteArray(String name, byte[] bytes) {
				// TODO Auto-generated method stub
				
			}

			public void setDepth(int depth) {
				// TODO Auto-generated method stub
				
			}

			public void setDepthBits(int depthBits) {
				// TODO Auto-generated method stub
				
			}

			public void setDouble(String name, double value) {
				// TODO Auto-generated method stub
				
			}

			public void setFloat(String name, float value) {
				// TODO Auto-generated method stub
				
			}

			public void setFramerate(int framerate) {
				// TODO Auto-generated method stub
				
			}

			public void setFrequency(int frequency) {
				// TODO Auto-generated method stub
				
			}

			public void setFullscreen(boolean fullscreen) {
				// TODO Auto-generated method stub
				
			}

			public void setHeight(int height) {
				// TODO Auto-generated method stub
				
			}

			public void setInt(String name, int value) {
				// TODO Auto-generated method stub
				
			}

			public void setLong(String name, long value) {
				// TODO Auto-generated method stub
				
			}

			public void setMusic(boolean musicEnabled) {
				// TODO Auto-generated method stub
				
			}

			public void setObject(String name, Object obj) {
				// TODO Auto-generated method stub
				
			}

			public void setRenderer(String renderer) {
				// TODO Auto-generated method stub
				
			}

			public void setSFX(boolean sfxEnabled) {
				// TODO Auto-generated method stub
				
			}

			public void setSamples(int samples) {
				// TODO Auto-generated method stub
				
			}

			public void setStencilBits(int stencilBits) {
				// TODO Auto-generated method stub
				
			}

			public void setVerticalSync(boolean vsync) {
				// TODO Auto-generated method stub
				
			}

			public void setWidth(int width) {
				// TODO Auto-generated method stub
				
			}
        	
        };
    	
//        defaults.setWidth(640);
//        defaults.setHeight(480);
//        defaults.setFrequency(60);
//        defaults.setDepth(16);
//        
//        defaults.setVerticalSync(false);
//        defaults.setFullscreen(false);
//        
//        defaults.setDepthBits(8);
//        defaults.setStencilBits(0);
//        defaults.setAlphaBits(0);
//        defaults.setSamples(0);
//        
//        defaults.setFramerate(60);
//        defaults.setRenderer("LWJGL");
    }
    
    protected static JmeContext current = null;
    protected static List<JmeContext> contexts = new ArrayList<JmeContext>();
    
    protected RenderContext<?> rc;
    protected Renderer renderer;
    protected int index;
    protected int type = CONTEXT_NONE;
    protected GameSettings settings = defaults;
    protected PassManager manager = new PassManager();
    
    protected int width, height, bpp, freq;
    protected boolean fullscreen, vsync;
    
    protected static ThreadLocal<JmeContext> contextLocal = new ThreadLocal<JmeContext>();
    protected static AtomicBoolean canvasDrawing = new AtomicBoolean(false);
    
    /**
     * Returns the default context implementor.
     * <p>
     * By default this returns the name of the LWJGL implementor.
     * <p>
     * For use with JmeContext.create(String,int)
     */
    public static String getDefaultImplementorClassName(){
        return "com.gibbon.jme.context.lwjgl.LWJGLContext";
    }
    
    /**
     * Creates a new context using the specified render system
     * <p>
     * The LWJGL rendering system interface is included with this API in the class LWJGLContext
     * <p>
     * The available options for the type paramater are:<br/>
     *   CONTEXT_NONE - Use a null context, 
     *      no GL calls are made and rendering never occurs.<br/>
     *   CONTEXT_WINDOW - The most common context type, 
     *      use JmeContext.getDisplay() to access the display's functions<br/>
     *   CONTEXT_CANVAS - Create an AWT Canvas for display, used for applets and editors<br/>
     *   CONTEXT_PBUFFER - Creates an offscreen drawing surface, to be used for headless apps
     *      to do offscreen rendering. If your application is not headless (e.g has a visible display) 
     *      use the standard jME TextureRenderer class as it will probably be faster.<br/>
     * <p>
     * @param renderSystem The render system class to use, for most cases you should specify LWJGLContext.class
     * @param type The type of context to create
     * @param parent Specifies the parent, or master context. Allows you to share textures and VBO data between contexts.
     *
     * @throws InstantiationException If the specified render system class cannot be initialized
     */
    public static JmeContext create(Class<? extends JmeContext> renderSystem, int type, JmeContext parent) throws InstantiationException{
        JmeContext cx = null;
        try {
            cx = renderSystem.newInstance();
        } catch (IllegalAccessException ex) {
            throw new InstantiationException("Cannot initialize JmeContext: "+ex.getLocalizedMessage());
        } 
        
        if (current==null)
            current = cx;
        
        contexts.add(cx);
        cx.index = contexts.size() - 1;
        cx.type = type;
        if (parent!=null)
                cx.setParentContext(parent);
        
        return cx;
    }
    
    /**
     * @see #create
     */
    public static JmeContext create(Class<? extends JmeContext> renderSystem, int type) throws InstantiationException{
        return create(renderSystem,type,null);
    }
    
    /**
     * Same as create(Class,int) but instead uses a string to look up the class.
     * 
     * @see #create
     * @throws InstantiationException If the rendersystem class could not be found or failed to initialize
     */
    public static JmeContext create(String renderSystemClassName, int type, JmeContext parent) throws InstantiationException {
        try {
            Class<?> clazz = Class.forName(renderSystemClassName);
            JmeContext cx = (JmeContext) clazz.newInstance();
            
            if (current==null) current = cx;
            contexts.add(cx);
            cx.index = contexts.size() - 1;
            cx.type = type;
            if (parent!=null)
                cx.setParentContext(parent);

            return cx;
        } catch (ClassNotFoundException ex) {
            ex.printStackTrace();
            throw new InstantiationException("Cannot initialize JmeContext: "+ex.getLocalizedMessage());
        } catch (IllegalAccessException ex) {
            throw new InstantiationException("Cannot initialize JmeContext: "+ex.getLocalizedMessage());
        }
    }
    
    /**
     * @see #create
     */
    public static JmeContext create(String renderSystemClassName, int type) throws InstantiationException{
        return create(renderSystemClassName,type,null);
    }
    
    /**
     * Returns the current context
     * <p>
     * If the current thread is a context thread, then this will return the thread's contex handle,
     * otherwise this will return the default context (usually the first one created, or one set with setDefault).
     */
    public static JmeContext get(){
        if (contextLocal.get()!=null)
            return contextLocal.get();
        
        return current;
    }
    
    /**
     * Returns a context handle by index
     */
    public static JmeContext get(int index){
        return contexts.get(index);
    }
    
    /**
     * Returns a list of all contexts active within the Java VM
     */
    public static List<JmeContext> getAllContexts(){
        return Collections.unmodifiableList(contexts);
    }
    
    public int getType(){
        return type;
    }
    
    /**
     * Returns true if the context owns the current thread.
     * <p>
     * <p>
     * CONTEXT_CANVAS: Does not have a context thread (uses AWT EventDispacher instead)
     * so this method will return false unless used from WITHIN the canvas render method.<p>
     * 
     * CONTEXT_PBUFFER: Does not have a context thread, instead it uses a user thread.
     * To bind the pbuffer context you must call getPbuffer().enterContext() and
     * exitContext() to unbind the pbuffer.<p>
     */
    public boolean inContext(){
        if (contextLocal.get()==this){
            if (type == CONTEXT_CANVAS){
                return canvasDrawing.get();
            }
            return true;
        }
        return false;
    }
    
    /**
     * Sets the display configuration to use for this context.
     * <p>
     * Canvas contexts only use the depthbits, alphabits, stencilbits and samples values.
     * Window contexts also use resolution, and if fullscreen is true, color depth and frequency are also used.
     * Framerate is always used, to ignore framerate, set value to 0
     * <p>
     * If this method is used when the display is already active,
     * then the display will automatically switch to use the new settings.
     */
    public void setSettings (GameSettings gs){
        settings = gs;
        if (isActive())
            restart();
    }
    
    /**
     * Returns the settings last set with setSettings()
     */
    public GameSettings getSettings(){
        return settings;
    }
    
    /**
     * Returns the render context. 
     * Used by jME classes, don't use externally.
     */
    public RenderContext<?> getRenderContext(){
        return rc;
    }
    
    /**
     * Returns this context's renderer.
     *
     * This will return null if the context is not active
     */
    public Renderer getRenderer(){
        return renderer;
    }
    
    /**
     * Returns the index of this context
     */
    public int getIndex(){
        return index;
    }
    
    @Override
    public String toString(){
        return getName()+"-"+getIndex();
    }
    
    /**
     * Returns the pass manager handle for this context
     */
    public PassManager getPassManager(){
        return manager;
    }
    
    /**
     * Executes the specified callable in the rendering thread
     *
     * Equivelant to getPassManager().execute(callable).get()
     */
    public <T> T execute(Callable<T> action){
        if (!isActive())
            return null;
        
        if (inContext()){
            try {
                return action.call();
            } catch (Exception ex) {
                logger.throwing("JmeContext","execute",ex);
            }
        }
        
        try {
            return getPassManager().execute(action).get();
        } catch (InterruptedException ex) {
            logger.throwing("JmeContext","execute",ex);
        } catch (ExecutionException ex) {
            logger.throwing("JmeContext","execute",ex);
        }
        return null;
    }
    
    public void executeLater(Callable<?> action){
        if (!isActive())
            return;
        
        if (inContext()){
            try {
                action.call();
                return;
            } catch (Exception ex) {
                logger.throwing("JmeContext","execute",ex);
            }
        }
        
        getPassManager().execute(action);
    }
    
    /**
     * Used internally to remove the context from the records after it has been disposed.
     */
    protected void remove(){
        contexts.remove(this);
        if (current==this){
            if (!contexts.isEmpty())
                current = contexts.get(0);
            else
                current = null;
        }
    }
    
    /**
     * Starts up this context
     * 
     * If CONTEXT_WINDOW is used, then a window will be created which can be accessed with getDisplay()
     * If CONTEXT_CANVAS is used, a canvas will be created, 
     *      it MUST be attached to a frame or applet to initialize.
     *      If you never attach the canvas, waitFor will hold forever causing a deadlock.
     * If CONTEXT_PBUFFER is used, a pbuffer is created which can be accessed with getPbuffer()
     * 
     * In both cases, the user must not make any GL calls until the context becomes active (see isActive).
     * For a convinience, the method waitFor will hold the current thread until the display or canvas becomes active.
     */
    public abstract void start();
    
    /**
     * Sets the context's parent/master.
     * Must be used before the context is initialized with start()
     *
     * @see #create
     * @throws IllegalStateException If start() has been called on this context
     */
    public abstract void setParentContext(JmeContext parent);
    
    /**
     * Returns the context with which this context shares resources with.
     */
    public abstract JmeContext getParentContext();
    
    /**
     * Returns the name of this context's implementor
     */
    public abstract String getName();
    
    /**
     * Returns true if the display or canvas is active.
     */
    public abstract boolean isActive();
    
    /**
     * Waits until the canvas or display starts rendering/becomes active.
     *
     * @throws InterruptedException If the current thread becomes interrupted while waiting.
     */
    public abstract void waitFor() throws InterruptedException;
    
    /**
     * Returns a jME timer for this context's implementor
     *
     * Returns null if start() was not called for this context
     * 
     * @return A jME timer
     */
    public abstract Timer getTimer();
    
    /**
     * Returns the display handle.
     *
     * Returns null if start() was not called or type is not CONTEXT_WINDOW
     */
    public abstract IJmeDisplay getDisplay();
    
    /**
     * Returns the canvas handle.
     *
     * Returns null if start() was not called or type is not CONTEXT_CANVAS. 
     */
    public abstract Canvas getCanvas();
    
    /**
     * Returns the pbuffer handle.
     * 
     * Returns null if start() was not called or type is not CONTEXT_PBUFFER.
     */
    public abstract IJmePixelBuffer getPbuffer();
    
    /**
     * Pbuffer context might not be supported on the target system.
     * 
     * @return true if PixelBuffer contexts can be created
     */
    public abstract boolean isPbufferSupported();
    
    /**
     * Restarts the JmeContext, used internally to re-initialize the context after a call to setSettings
     *
     * @throws JmeException If the restart failed
     */
    public abstract void restart();
    
    /**
     * Disposes this context.
     *
     * For CONTEXT_DISPLAY: The display closes down, the context thread is destroyed.
     * For CONTETX_CANVAS: Rendering stops completely, but the canvas remains. It is up to the user to dispose the canvas.
     */
    public abstract void dispose();
}
