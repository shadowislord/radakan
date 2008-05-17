package com.gibbon.radakan;

import com.gibbon.jme.context.JmeContext;
import com.jme.renderer.Camera;
import com.jme.renderer.Renderer;
import com.jme.scene.Skybox;
import com.jme.scene.Skybox.Face;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;

public class DefaultWorld {

    private DefaultWorld() {
    }
    
    public static Skybox loadSkybox(final JmeContext cx){
        Skybox sky = new Skybox("sky", 2f, 2f, 2f){
            @Override
            public void updateGeometricState(float tpf, boolean initiator){
                Camera cam = cx.getRenderer().getCamera();
                setLocalTranslation(cam.getLocation());
                super.updateGeometricState(tpf, initiator);
            }
        };
        
        sky.setTexture(Face.Down,  Setup.loadTexture("cloud_down.jpg", true));
        sky.setTexture(Face.Up,    Setup.loadTexture("cloud_up.jpg", true));
        
        sky.setTexture(Face.West,  Setup.loadTexture("cloud_1.jpg", true));
        sky.setTexture(Face.North, Setup.loadTexture("cloud_2.jpg", true));
        sky.setTexture(Face.East,  Setup.loadTexture("cloud_4.jpg", true));
        sky.setTexture(Face.South, Setup.loadTexture("cloud_3.jpg", true));
        
        sky.setRenderQueueMode(Renderer.QUEUE_SKIP);
        
        ZBufferState zbuf = cx.getRenderer().createZBufferState();
        zbuf.setFunction(TestFunction.Always);
        zbuf.setWritable(false);
        sky.setRenderState(zbuf);
        
        sky.updateRenderState();
        
        return sky;
    }

}
