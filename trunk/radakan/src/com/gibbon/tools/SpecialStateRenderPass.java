package com.gibbon.tools;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.RenderPass;
import com.jme.light.DirectionalLight;
import com.jme.light.PointLight;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.renderer.ColorRGBA;
import com.jme.renderer.Renderer;
import com.jme.renderer.lwjgl.LWJGLRenderer;
import com.jme.scene.state.CullState;
import com.jme.scene.state.LightState;
import com.jme.scene.state.MaterialState;
import com.jme.scene.state.MaterialState.ColorMaterial;
import com.jme.scene.state.RenderState;
import com.jme.scene.state.WireframeState;
import com.jme.scene.state.WireframeState.Face;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;

public class SpecialStateRenderPass extends RenderPass {

    private WireframeState ws;
    private MaterialState ms;
    private ZBufferState zbuf;
    private CullState cull;
    private LightState ls;
    private PointLight camLight;
    
    public enum SpecialState {
        
        /**
         * Model is drawn in wireframe
         */
        WIREFRAME,
        
        /**
         * Model is drawn with default lighting
         */
        SOLID,
        
        /**
         * Model is drawn with textures only
         */
        TEXTURED,
        
        /**
         * Model is drawn without any state overriding (as default)
         */
        MATERIAL
        
    }
    
    @Override
    public void initPass(JmeContext cx){
        super.initPass(cx);
        
        ms = cx.getRenderer().createMaterialState();
        ms.setColorMaterial(ColorMaterial.None);
        
        ws = cx.getRenderer().createWireframeState();
        ws.setAntialiased(true);
        ws.setFace(Face.FrontAndBack);
        
        zbuf = cx.getRenderer().createZBufferState();
        zbuf.setFunction(TestFunction.LessThan);
        
        cull = cx.getRenderer().createCullState();
        cull.setCullFace(CullState.Face.Back);
        
        ls = cx.getRenderer().createLightState();
        ls.setSeparateSpecular(true);
        
        DirectionalLight dl = new DirectionalLight();
        dl.setEnabled(true);
        dl.setDirection(new Vector3f(0,0,1));
        dl.setDiffuse(new ColorRGBA(0.5f, 0.5f, 0.1f, 1.0f));
        ls.attach(dl);
        
        DirectionalLight dl2 = new DirectionalLight();
        dl2.setEnabled(true);
        dl.setDirection(new Vector3f(1,0.1f,0).normalizeLocal());
        dl.setDiffuse(new ColorRGBA(0.4f, 0.4f, 0.8f, 1.0f));
        ls.attach(dl2);
        
        camLight = new PointLight();
        camLight.setDiffuse(ColorRGBA.lightGray);
        camLight.setSpecular(ColorRGBA.white);
        ls.attach(camLight);
    }
    
    public void setForceDefaults(boolean enabled){
        if (enabled){
            for (int i = 0; i < RenderState.RS_MAX_STATE; i++){
                passStates[i] = Renderer.defaultStateList[i];
            }
        }else{
            for (int i = 0; i < RenderState.RS_MAX_STATE; i++){
                passStates[i] = null;
            }
        }
    }

    @Override
    public void doUpdate(JmeContext cx){
        super.doUpdate(cx);
        
        Camera cam = cx.getRenderer().getCamera();
        
        Vector3f temp = camLight.getLocation();
        temp.set(cam.getLeft()).multLocal(5.0f);
        temp.addLocal(cam.getLocation());
        camLight.setLocation(temp);
    }
    
    public void setState(SpecialState state){
        switch (state){
            case WIREFRAME:
                setForceDefaults(true);
                setPassState(ws);
                setPassState(ms);
                break;
            case SOLID:
                setForceDefaults(true);
                setPassState(ls);
                setPassState(zbuf);
                setPassState(cull);
                break;
            case TEXTURED:
                setForceDefaults(true);
                setPassState(zbuf);
                setPassState(cull);
                setPassState(ms);
                passStates[RenderState.RS_TEXTURE] = null;
                break;
            case MATERIAL:
                setForceDefaults(false);
        }
    }
    
}
