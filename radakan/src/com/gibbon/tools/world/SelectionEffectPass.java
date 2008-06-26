package com.gibbon.tools.world;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.PassType;
import com.gibbon.jme.context.RenderPass;
import com.gibbon.radakan.entity.Entity;
import com.gibbon.radakan.entity.unit.ModelUnit;
import com.jme.renderer.ColorRGBA;
import com.jme.renderer.Renderer;
import com.jme.scene.Spatial;
import com.jme.scene.state.CullState;
import com.jme.scene.state.CullState.Face;
import com.jme.scene.state.MaterialState;
import com.jme.scene.state.MaterialState.ColorMaterial;
import com.jme.scene.state.MaterialState.MaterialFace;
import com.jme.scene.state.RenderState;
import com.jme.scene.state.WireframeState;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;
import java.util.List;

public class SelectionEffectPass extends RenderPass {

    public SelectionEffectPass(){
        super(PassType.PRE_RENDER, "SelectionEffect");
        Renderer r = JmeContext.get().getRenderer();
        
//        CullState cs = r.createCullState();
//        cs.setCullFace(Face.Front);
//        setPassState(cs);
        
        WireframeState ws = r.createWireframeState();
        ws.setFace(WireframeState.Face.FrontAndBack);
        ws.setLineWidth(10.0f);
        setPassState(ws);
        
        MaterialState ms = r.createMaterialState();
        ms.setEmissive(ColorRGBA.pink);
        ms.setColorMaterial(ColorMaterial.None);
        ms.setMaterialFace(MaterialFace.FrontAndBack);
        setPassState(ms);
        
        ZBufferState zs = r.createZBufferState();
        zs.setFunction(TestFunction.Always);
        zs.setWritable(true);
        setPassState(zs);
    }
    
    @Override
    public void initPass(JmeContext cx){
        for (int i = 0; i < RenderState.RS_MAX_STATE; i++){
            if (passStates[i] == null && Renderer.defaultStateList[i] != null)
                setPassState(Renderer.defaultStateList[i]);
        }
    }
    
    @Override
    public void doRender(JmeContext cx){
        List<Entity> entities = EditorState.getState().selection;
        
        for (Entity ent: entities){
            ModelUnit unit = ent.getUnit(ModelUnit.class);
            cx.getRenderer().draw(unit.getModel());
        }
        
        if (entities.size() != 0) cx.getRenderer().renderQueue();
    }
    
}
