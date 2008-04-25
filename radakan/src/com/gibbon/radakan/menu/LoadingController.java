package com.gibbon.radakan.menu;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.PassType;
import com.gibbon.radakan.Main;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Controller;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import java.util.concurrent.atomic.AtomicBoolean;

public class LoadingController extends Controller {

    private ColorRGBA color = new ColorRGBA(ColorRGBA.white);
    
    private boolean loadingComplete = false;
    
    private float fadeTime = 0f;
    private float totalFadeTime = 1f;
    
    private float loadTime = 0f;
    private float totalLoadTime = 2f;
    
    private Spatial loading;
    private AtomicBoolean loadThreadDone = new AtomicBoolean(false);
    
    public LoadingController(Spatial loading){
        this.loading = loading;
        setAlphaColor(loading);
    }
    
    public void notifyComplete(){
        loadThreadDone.set(true);
    }
    
    private void setAlphaColor(Spatial spatial){
        if (spatial instanceof Node){
            Node n = (Node) spatial;
            for (Spatial s: n.getChildren()){
                setAlphaColor(s);
            }
        }else if (spatial instanceof TriMesh){
            TriMesh mesh = (TriMesh) spatial;
            mesh.setDefaultColor(color);
        }
    }
    
    @Override
    public void update(float tpf) {
        if (loadingComplete){
            fadeTime += 0.016666;
            color.a = 1f - (fadeTime / totalFadeTime);
            if (fadeTime > totalFadeTime){
                // detach pass from PassManager
                // will cause this and the other
                // stuff to be garbage collected
                JmeContext.get().getPassManager().remove(PassType.RENDER, "menu");
            }
        }else{
            // update bar, etc
            loadTime += tpf;
            if (loadTime > totalLoadTime && loadThreadDone.get()){
                loadingComplete = true;
                // fade into scene
                Main.doAttachScene(JmeContext.get());
            }
        }
    }

}
