/*
 * Copyright (c) 2008, OgreLoader
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

package com.radakan.jme.mxml;

import com.jme.renderer.Renderer;
import com.jme.scene.Spatial;
import com.jme.scene.Spatial.LightCombineMode;
import com.jme.scene.state.RenderState;
import com.jme.system.DisplaySystem;

/**
 * Represents a single Ogre3D material object.
 */
public final class Material {
    
    private final String name;
    private final RenderState[] states = new RenderState[RenderState.RS_MAX_STATE];
    
    boolean recieveShadows = false;
    boolean transparent = false;
    boolean lightingOff = false;
    
    public Material(String name) {
        this.name = name;
    }
    
    public RenderState getState(int stateType){
        if (states[stateType] == null){
            states[stateType] = DisplaySystem.getDisplaySystem().getRenderer().createState(stateType);
        }
        return states[stateType];
    }
    
    public void apply(Spatial obj){
        if (transparent)
            obj.setRenderQueueMode(Renderer.QUEUE_TRANSPARENT);
        else 
            obj.setRenderQueueMode(Renderer.QUEUE_OPAQUE);
        
        if (lightingOff)
            obj.setLightCombineMode(LightCombineMode.Off);
        
        for (int i = 0; i < states.length; i++)
            obj.setRenderState(states[i]);
        
    }
    
    @Override
    public String toString(){
        return name;
    }
    
}
