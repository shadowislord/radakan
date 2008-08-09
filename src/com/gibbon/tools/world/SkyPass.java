package com.gibbon.tools.world;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.Pass;
import com.gibbon.jme.context.PassType;
import com.jme.renderer.Camera;
import com.jme.renderer.Renderer;
import com.jme.scene.Spatial;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;

public class SkyPass extends Pass {

    private Spatial sky;
    
    public SkyPass(){
        super(PassType.PRE_RENDER, "Sky");
    }
    
    public void setSky(Spatial sky){
        this.sky = sky;
        sky.setRenderQueueMode(Renderer.QUEUE_SKIP);
    }
    
    public Spatial getSky(){
        return sky;
    }
    
    @Override
    public void runPass(JmeContext context) {
        Camera cam = context.getRenderer().getCamera();
        sky.setLocalTranslation(cam.getLocation().x, cam.getLocation().y - 1f, cam.getLocation().z);
        sky.updateGeometricState(0, true);
        
        context.getRenderer().draw(sky);
    }

    @Override
    public void initPass(JmeContext context) {
        ZBufferState zbuf = context.getRenderer().createZBufferState();
        zbuf.setWritable(false);
        zbuf.setFunction(TestFunction.Always);
        sky.setRenderState(zbuf);
    }

    @Override
    public void cleanPass(JmeContext context) {
    }

    
    
}
