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

package com.gibbon.meshparser;

import com.gibbon.jme.context.JmeContext;
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
            if (JmeContext.get() == null){
                states[stateType] = DisplaySystem.getDisplaySystem().getRenderer().createState(stateType);
            }else{
                states[stateType] = JmeContext.get().getRenderer().createState(stateType);
            }
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
