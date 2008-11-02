package com.radakan.game.util;

import com.jme.scene.Spatial;

public interface IShadowManager {

    public void addOccluder(Spatial occluder);
    public void addShadowReciever(Spatial reciever);
    
    public void removeOccluder(Spatial occluder);
    public void removeShadowReciever(Spatial reciever);
    
    public void update(float tpf);
    
}
