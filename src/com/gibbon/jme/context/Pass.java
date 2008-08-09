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
