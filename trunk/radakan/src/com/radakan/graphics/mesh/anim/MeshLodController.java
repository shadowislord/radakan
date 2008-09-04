package com.radakan.graphics.mesh.anim;

import com.jme.renderer.Camera;
import com.jme.scene.Controller;
import com.jme.system.DisplaySystem;

public class MeshLodController extends Controller {

    private MeshAnimationController animControl;
    
    public MeshLodController(MeshAnimationController animControl){
        this.animControl = animControl;
    }
    
    @Override
    public void update(float time) {
        OgreMesh[] targets = animControl.getMeshList();
        int maxLod = targets[0].getLodLevelCount();
        
        Camera cam = DisplaySystem.getDisplaySystem().getRenderer().getCamera();
        targets[0].updateWorldVectors();
        float dist = cam.getLocation().distance(targets[0].getWorldTranslation());
        int level = Math.round(dist / 200f);
        if (level < 0)
            level = 0;
        else if (level > maxLod)
            level = maxLod;
            
        //animControl.setFrameSkip(level);
        for (OgreMesh target: targets){
            target.setLodLevel(level);
        }
    }

    
    
}
