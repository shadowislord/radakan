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
import com.jme.renderer.RenderContext;
import com.jme.scene.Spatial;
import com.jme.scene.state.RenderState;
import java.util.ArrayList;

/**
 * A pass responsible for rendering objects to the framebuffer
 * 
 * @author Momoko_Fan
 */
public class RenderPass extends Pass {

    protected ArrayList<Spatial> spatials = new ArrayList<Spatial>();
    protected float zFactor;
    protected float zOffset;
    
    protected RenderState[] passStates = new RenderState[RenderState.RS_MAX_STATE];
    protected RenderState[] savedStates = new RenderState[RenderState.RS_MAX_STATE];

    /**
     * @param type The type, or bucket where to execute the pass
     * @param name Name of the pass. Used for debugging.
     */
    public RenderPass(PassType type, String name){
        super(type,name);
    }
    
    /**
     * @param name Name of the pass. Used for debugging.
     */
    public RenderPass(String name){
        this(PassType.RENDER,name);
    }
    
    /**
     * Creates a new RenderPass
     */
    public RenderPass(){
        this("render");
    }
    
    /**
     * Callback used by subclasses to initialize themselves in the rendering thread.
     * 
     * @param context
     */
    public void initPass(JmeContext context) {
    }
    
    /**
     * Callback used by subclasses to dispose resources when the rendering thread is terminated
     * 
     * @param context
     */
    public void cleanPass(JmeContext context){
    }
    
    /**
     * Executes this pass. Subclasses should override doUpdate and doRender instead of this method.
     * @param jc
     */
    public void runPass(JmeContext jc) {
        doUpdate(jc);
        RenderContext<?> rc  = jc.getRenderContext();
        applyPassStates(rc);
        jc.getRenderer().setPolygonOffset(zFactor,zOffset);
        doRender(jc);
        jc.getRenderer().clearPolygonOffset();
        resetOldStates(rc);
    }

    public void setPassState(RenderState state) {
        passStates[state.getType()] = state;
    }

    public void clearPassState(int renderStateType) {
        passStates[renderStateType] = null;
    }

    public void clearPassStates() {
        for (int i = 0; i < passStates.length; i++)
            passStates[i] = null;
    }

    protected void applyPassStates(RenderContext<?> rc) {
        for (int x = RenderState.RS_MAX_STATE; --x >= 0; ) {
            if (passStates[x] != null) {
                savedStates[x] = rc.enforcedStateList[x];
                rc.enforcedStateList[x] = passStates[x];
            }
        }
    }
    
    protected void resetOldStates(RenderContext<?> rc) {
        for (int x = RenderState.RS_MAX_STATE; --x >= 0; ) {
            if (passStates[x] != null) {
                rc.enforcedStateList[x] = savedStates[x];
            }
        }
    }
    
    protected void doRender(JmeContext jc){
        for (Spatial spat: spatials){
            jc.getRenderer().draw(spat);
        }
        jc.getRenderer().renderQueue();
    }
    
    protected void doUpdate(JmeContext jc){
        for (Spatial spat: spatials){
            spat.updateGeometricState(jc.getTimer().getTimePerFrame(),true);
        }
    }
    
    public void add(Spatial toAdd) {
        spatials.add(toAdd);
    }
    
    public Spatial get(int index) {
        return spatials.get(index);
    }

    public boolean contains(Spatial s) {
        return spatials.contains(s);
    }
    
    public boolean remove(Spatial toRemove) {
        return spatials.remove(toRemove);
    }
    
    public int size() {
        return spatials.size();
    }
    
    /**
     * @return Returns the zFactor.
     */
    public float getZFactor() {
        return zFactor;
    }

    /**
     * Sets the polygon offset param - factor - for this Pass.
     * 
     * @param factor
     *            The zFactor to set.
     */
    public void setZFactor(float factor) {
        zFactor = factor;
    }

    /**
     * @return Returns the zOffset.
     */
    public float getZOffset() {
        return zOffset;
    }

    /**
     * Sets the polygon offset param - offset - for this Pass.
     * 
     * @param offset
     *            The zOffset to set.
     */
    public void setZOffset(float offset) {
        zOffset = offset;
    }

}
