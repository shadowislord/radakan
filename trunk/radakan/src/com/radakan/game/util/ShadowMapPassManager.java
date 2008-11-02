package com.radakan.game.util;

import com.jme.renderer.Camera;
import com.jme.scene.Spatial;
import com.jme.system.DisplaySystem;
import com.radakan.graphics.util.DirectionalShadowMapPass;


public class ShadowMapPassManager implements IShadowManager {

    private DirectionalShadowMapPass sPass;
    
    public ShadowMapPassManager(DirectionalShadowMapPass pass){
        sPass = pass;
    }
    
    public void addOccluder(Spatial occluder) {
        sPass.addOccluder(occluder);
        System.out.println("ADDED OCCLUDER: "+occluder);
    }

    public void addShadowReciever(Spatial reciever) {
        sPass.add(reciever);
    }
    
    public void removeOccluder(Spatial occluder){
        //?
    }
    
    public void removeShadowReciever(Spatial reciever){
        sPass.remove(reciever);
    }

    public void update(float tpf) {
        Camera cam = DisplaySystem.getDisplaySystem().getRenderer().getCamera();
        sPass.setViewTarget(cam.getLocation());
    }

}
