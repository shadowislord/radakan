package com.gibbon.meshparser;

import com.gibbon.jme.context.JmeContext;
import com.jme.renderer.Renderer;
import com.jme.scene.SceneElement;
import com.jme.scene.Spatial;
import com.jme.scene.state.RenderState;
import com.jme.system.DisplaySystem;


public final class Material {
    
    private final String name;
    private final RenderState[] states = new RenderState[RenderState.RS_MAX_STATE];
    
    boolean recieveShadows = false;
    boolean transparent = false;
    
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
    
    public void apply(SceneElement obj){
        if (transparent)
            obj.setRenderQueueMode(Renderer.QUEUE_TRANSPARENT);
        else 
            obj.setRenderQueueMode(Renderer.QUEUE_OPAQUE);
        
        for (int i = 0; i < states.length; i++)
            obj.setRenderState(states[i]);
    }
    
    @Override
    public String toString(){
        return name;
    }
    
}
