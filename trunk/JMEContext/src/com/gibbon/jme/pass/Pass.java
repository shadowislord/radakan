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

package com.gibbon.jme.pass;

import com.gibbon.jme.context.JmeContext;
import com.jme.util.GameTaskQueue;
import java.util.concurrent.Callable;
import java.util.concurrent.Future;

/**
 * A pass is an action which happens each time at the game loop.
 *
 * Passes are used within the context system to do tasks such as
 * input polling, scene update, and rendering.
 */
public abstract class Pass {
    
    protected boolean enabled = true;
    
    protected String name;
    protected PassType type;
    
    protected GameTaskQueue queue = new GameTaskQueue();
    
    boolean needInit = false;
    protected Object initLock = new Object();
    
    /**
     * Creates a new pass with the specified priority and name
     */
    public Pass(PassType type, String name){
        this.name=name;
        this.type=type;
    }
    
    /**
     * Returns the type of this pass
     */
    public PassType getType(){
        return type;
    }
    
    /**
     * Returns true if this pass is run at the game loop
     */
    public boolean isEnabled(){
        return enabled;
    }
    
    /**
     * Disables or enables this pass.
     *
     * Disabled passes are not run during the game loop.
     */
    public void setEnabled(boolean enabled){
        this.enabled=enabled;
    }
    
    /**
     * The name of this pass. 
     */
    public String getName(){
        return name;
    }
    
    /**
     * Runs the pass
     */
    public void run(JmeContext context){
        queue.execute();
        runPass(context);
    }
    
    /**
     * Executes code in the context of the pass
     *
     * The code is executed before the pass code.
     * WARNING: 
     * Make sure that the pass is enabled when you need it,
     * otherwise Future.get() will hold until you enable it.
     */
    public <T> Future<T> execute(Callable<T> action){
        return queue.enqueue(action);
    }
    
    /**
     * Returns a string describing the pass
     */
    @Override
    public String toString(){
        return getClass().getSimpleName()+"["+getName()+"]";
    }
    
    /**
     * runs the user code for this pass
     * 
     * The user should implement this method to insert custom code.
     * PassManagers should call run() instead.
     */
    public abstract void runPass(JmeContext context);
    
    /**
     * Called from the context thread to initialize the pass
     */
    public abstract void initPass(JmeContext context);
    
    /**
     * Called to cleanup resources
     */
    public abstract void cleanPass(JmeContext context);
    
    /**
     * Waits until the pass is initialized
     */
    public void waitFor(){
        synchronized (initLock){
            while (needInit){
                try {
                    initLock.wait();
                } catch (InterruptedException ex) {
                    ex.printStackTrace();
                }
            }
        }
    }
}
