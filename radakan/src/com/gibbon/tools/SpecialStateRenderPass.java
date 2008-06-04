package com.gibbon.tools;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.RenderPass;
import com.jme.light.DirectionalLight;
import com.jme.light.PointLight;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.renderer.ColorRGBA;
import com.jme.renderer.Renderer;
import com.jme.scene.Spatial;
import com.jme.scene.state.CullState;
import com.jme.scene.state.LightState;
import com.jme.scene.state.MaterialState;
import com.jme.scene.state.MaterialState.ColorMaterial;
import com.jme.scene.state.MaterialState.MaterialFace;
import com.jme.scene.state.RenderState;
import com.jme.scene.state.WireframeState;
import com.jme.scene.state.WireframeState.Face;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;
import com.jme.util.BoneDebugger;
import com.jme.util.geom.Debugger;

public class SpecialStateRenderPass extends RenderPass {

    private WireframeState ws;
    private MaterialState ms;
    private MaterialState nms;
    private ZBufferState zbuf;
    private CullState cull;
    private LightState ls;
    private LightState nls;
    private PointLight camLight;
    private SpecialState state = SpecialState.MATERIAL;
    
    private boolean bones = false;
    private boolean normals = false;
    private boolean bounds = false;
    
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
        
        Debugger.setBoundsColor(ColorRGBA.blue);
        
        ms = cx.getRenderer().createMaterialState();
        ms.setColorMaterial(ColorMaterial.None);
        ms.setMaterialFace(MaterialFace.FrontAndBack);

        nms = cx.getRenderer().createMaterialState();
        nms.setColorMaterial(ColorMaterial.None);
        nms.setMaterialFace(MaterialFace.FrontAndBack);
        nms.setEmissive(ColorRGBA.white);
        
        ws = cx.getRenderer().createWireframeState();
        ws.setAntialiased(true);
        ws.setFace(Face.FrontAndBack);
        
        zbuf = cx.getRenderer().createZBufferState();
        zbuf.setFunction(TestFunction.LessThan);
        
        cull = cx.getRenderer().createCullState();
        cull.setCullFace(CullState.Face.Back);
        
        nls = cx.getRenderer().createLightState();
        PointLight dummy = new PointLight();
        nls.attach(dummy);
        
        ls = cx.getRenderer().createLightState();
        ls.setSeparateSpecular(true);
        
        DirectionalLight dl = new DirectionalLight();
        dl.setEnabled(true);
        dl.setDirection(new Vector3f(0,0,-1).normalizeLocal());
        dl.setDiffuse(new ColorRGBA(0.8f, 0.8f, 0.4f, 1.0f));
        ls.attach(dl);
        
        DirectionalLight dl2 = new DirectionalLight();
        dl2.setEnabled(true);
        dl.setDirection(new Vector3f(1,0.1f,0).normalizeLocal());
        dl.setDiffuse(new ColorRGBA(0.4f, 0.4f, 0.8f, 1.0f));
        ls.attach(dl2);
        
        camLight = new PointLight();
        camLight.setEnabled(true);
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

    public void setNormals(boolean normals){
        this.normals = normals;
    }
    
    public void setBounds(boolean bounds){
        this.bounds = bounds;
    }
    
    public void setBones(boolean bones){
        this.bones = bones;
    }
    
    @Override
    public void doUpdate(JmeContext cx){
        super.doUpdate(cx);
        
        Camera cam = cx.getRenderer().getCamera();
        
        Vector3f temp = camLight.getLocation();
        temp.set(cam.getLeft()).multLocal(5.0f);
        temp.addLocal(cam.getLocation());
        camLight.setLocation(temp);
        //i think a combo of concept art pans + terrain pans + other stuff
    }
    
    @Override
    public void runPass(JmeContext cx){
        super.runPass(cx);
        
        if (bones){
            for (Spatial spatial: spatials){
                BoneDebugger.drawBones(spatial, cx.getRenderer());
            }
        }
        if (normals){
           for (Spatial spatial: spatials){
                Debugger.drawNormals(spatial, cx.getRenderer(), -1f, true);
            } 
        }
        if (bounds){
            for (Spatial spatial: spatials){
                Debugger.drawBounds(spatial, cx.getRenderer(), true);
            } 
        }
    }
    
    public void clear(){
        spatials.clear();
    }
    
    public SpecialState getState(){
        return state;
    }
    
    public void setState(SpecialState state){
        this.state = state;
        
        switch (state){
            case WIREFRAME:
                setForceDefaults(true);
                setPassState(ws);
                setPassState(nms);
                setPassState(nls);
                break;
            case SOLID:
                setForceDefaults(true);
                setPassState(ls);
                setPassState(zbuf);
                break;
            case TEXTURED:
                setForceDefaults(true);
                setPassState(zbuf);
                setPassState(ms);
                passStates[RenderState.RS_TEXTURE] = null;
                break;
            case MATERIAL:
                setForceDefaults(false);
                break;
        }
        
        for (Spatial model : spatials){
            model.updateRenderState();
        }
    }
    
}
