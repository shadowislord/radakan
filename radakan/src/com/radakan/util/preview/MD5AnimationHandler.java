package com.radakan.util.preview;

import com.model.md5.JointAnimation;
import com.model.md5.controller.JointController;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

public class MD5AnimationHandler implements IAnimationHandler {

    private JointController control;
    
    private Map<String, JointAnimation> animMap = new HashMap<String, JointAnimation>();
    
    public MD5AnimationHandler(JointController control){
        this.control = control;
        
        for (JointAnimation anim : this.control.getAnimations()){
            animMap.put(anim.getName(), anim);
        }
    }
    
    
    public Collection<String> list() {
        return animMap.keySet();
    }

    public float getLength(String name) {
        JointAnimation anim = animMap.get(name);
        
        if (anim == null)
            return -1;
        
        return anim.getAnimationTime();
    }

    public void play(String name) {
        control.setActiveAnimation(name);
    }

    public void blendTo(String name, float time) {
//        control.setActiveAnimation(name, time);
    }

    public void setSpeed(float speed) {
        control.setSpeed(speed);
    }

    public float getSpeed() {
        return control.getSpeed();
    }

    public void reset() {
        control.setActiveAnimation((String)null);
    }

    public String getCurrent() {
        return control.getActiveAnimation().getName();
    }


}
