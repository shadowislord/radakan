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
import com.jme.input.FirstPersonHandler;
import com.jme.util.GameTaskQueue;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.Future;

/**
 * PassManager is used by JmeContexts to manager passes
 * 
 * @author Momoko_Fan
 */
@SuppressWarnings("unchecked")
public class PassManager {
    
    private List<Pass>[] buckets = new ArrayList[PassType.POST_RENDER.priority+1];
    
    private GameTaskQueue queue = new GameTaskQueue();
    private float tpf = -1f;
    
    public PassManager(){
        queue.setExecuteAll(true);
        for (int i = 0; i < buckets.length; i++){
            buckets[i] = new ArrayList<Pass>();
        }
    }
    
    /**
     * Execute the callable in the pass
     * @param action The callable to execute
     * @return A future object to get the result
     */
    public <V> Future<V> execute(Callable<V> action){
        return queue.enqueue(action);
    }
    
    /**
     * Returns the time in seconds since the last frame
     */
    public float getTPF(){
        return tpf;
    }

    public void add(final Pass pass){
        pass.needInit = true;
        queue.enqueue(new Callable<Object>(){
            public Object call(){
                buckets[pass.getType().priority].add(pass);
                return null;
            }
        });
    }
    
    public void remove(Pass pass){
        buckets[pass.getType().priority].remove(pass);
    }
    
    public Pass getPass(String passName){
        for (List<Pass> passList : buckets){
            for (Pass p : passList){
                if (p.getName().equals(passName))
                    return p;
            }
        }
        
        return null;
    }
    
    public void remove(PassType type, String passName){
        for (Pass p: buckets[type.priority]){
            if (p.getName().equals(passName)){
                remove(p);
                return;
            }
        }
    }
    
    public boolean contains(Pass pass){
        return buckets[pass.getType().priority].contains(pass);
    }
    
    public void cleanup(JmeContext context){
        for (int bucket_i = 0; bucket_i < buckets.length; bucket_i++){
            Iterator<Pass> it = buckets[bucket_i].iterator();
            while (it.hasNext()){
                it.next().cleanPass(context);
            }
        }
    }
    
    /**
     * Execute the passes.<br/>
     * Should only be used internally.
     * @param context The context to run in
     */
    public void run(JmeContext context) {
        tpf = context.getTimer().getTimePerFrame();
        
        queue.execute();

        for (int bucket_i = 0; bucket_i < buckets.length; bucket_i++){
            Iterator<Pass> it = buckets[bucket_i].iterator();
            while (it.hasNext()){
                Pass p = it.next();
                if (p.needInit){
                    p.needInit = false;
                    p.initPass(context);
                    synchronized (p.initLock){
                        p.initLock.notifyAll();
                    }
                }
                if (p.isEnabled()){
                    p.run(context);
                }
            }
        }
        
    }

    /**
     * Load several passes to provide SimpleGame like functionality.
     */
    /*public void loadDefaultPasses() {
        execute(new Callable<Object>(){
            public Object call(){
                add(new InputPass(new FirstPersonHandler(JmeContext.get().getRenderer().getCamera(), 10, 1), false));
                add(new ExitListenerPass(
                        new Callable<Object>(){
                            public Object call(){
                                JmeContext.get().dispose();
                                System.exit(0);
                                return null;
                            }
                        }));
                
                return null;
            }
        });
    }*/

}
