package com.radakan.util.preview;

import com.radakan.graphics.mesh.anim.MeshAnimationController;
import java.util.Collection;

public class OgreAnimationHandler implements IAnimationHandler {

    private MeshAnimationController control;
    
    public OgreAnimationHandler(MeshAnimationController control){
        this.control = control;
    }
    
    public Collection<String> list() {
        return control.getList();
    }

    public float getLength(String name) {
        return control.getAnimationLength(name);
    }

    public void play(String name) {
        control.setAnimation(name);
    }

    public void blendTo(String name, float time) {
        
    }

    public void setSpeed(float speed) {
        control.setSpeed(speed);
    }

    public float getSpeed() {
        return control.getSpeed();
    }

    public String getCurrent() {
        return control.getActiveAnimation();
    }

    
    
}
