package com.radakan.util.preview;

import java.util.Collection;

public interface IAnimationHandler {

    public Collection<String> list();
    
    public float getLength(String name);
    
    public void play(String name);
    
    public void blendTo(String name, float time);
    
    public void setSpeed(float speed);
    
    public float getSpeed();
    
    public String getCurrent();
    
}
