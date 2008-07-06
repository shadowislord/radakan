package com.gibbon.tools.world;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.PassType;
import com.gibbon.jme.context.RenderPass;
import com.gibbon.radakan.entity.Entity;
import com.gibbon.radakan.entity.unit.ModelUnit;
import com.jme.math.FastMath;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import com.jme.renderer.Renderer;
import com.jme.renderer.pass.OutlinePass;
import com.jme.scene.Line;
import com.jme.scene.Line.Mode;
import com.jme.scene.Spatial;
import com.jme.scene.Spatial.CullHint;
import com.jme.scene.state.BlendState;
import com.jme.scene.state.BlendState.DestinationFunction;
import com.jme.scene.state.BlendState.SourceFunction;
import com.jme.scene.state.CullState;
import com.jme.scene.state.CullState.Face;
import com.jme.scene.state.LightState;
import com.jme.scene.state.MaterialState;
import com.jme.scene.state.MaterialState.ColorMaterial;
import com.jme.scene.state.MaterialState.MaterialFace;
import com.jme.scene.state.RenderState;
import com.jme.scene.state.WireframeState;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;
import java.awt.event.MouseEvent;
import java.nio.FloatBuffer;
import java.util.List;

public class SelectionEffectPass extends RenderPass {

    private static RenderState[] selectionMaterial = new RenderState[RenderState.RS_MAX_STATE];
    private static RenderState[] placementMaterial = new RenderState[RenderState.RS_MAX_STATE];
    private static RenderState[] linesMaterial     = new RenderState[RenderState.RS_MAX_STATE];
    private static RenderState[] postSelectionMaterial = new RenderState[RenderState.RS_MAX_STATE]; 
    
    private Vector3f point = new Vector3f();
    private Line brushLines = new Line("Brush");
    private int brushRadius = -1;
    
    public SelectionEffectPass(){
        super(PassType.POST_RENDER, "SelectionEffect");
        Renderer r = JmeContext.get().getRenderer();
        
        // -- Selection Material --
//        CullState cs = r.createCullState();
//        cs.setCullFace(Face.Back);
//        selectionMaterial[cs.getType()] = cs;
        
        WireframeState ws = r.createWireframeState();
        ws.setFace(WireframeState.Face.FrontAndBack);
        ws.setLineWidth(2.0f);
        ws.setAntialiased(true);
        selectionMaterial[ws.getType()] = ws;
        
        LightState ls = r.createLightState();
        ls.setGlobalAmbient(ColorRGBA.green);
        selectionMaterial[ls.getType()] = ls;
        
//        ZBufferState zs = r.createZBufferState();
//        zs.setFunction(TestFunction.LessThan);
//        zs.setWritable(true);
//        selectionMaterial[zs.getType()] = zs;
        
        BlendState bs = r.createBlendState();
        bs.setBlendEnabled(true);
        bs.setSourceFunction(SourceFunction.SourceAlpha);
        bs.setDestinationFunction(DestinationFunction.OneMinusSourceAlpha);
        selectionMaterial[bs.getType()] = bs;
        
        // -- PostSelection Material
        CullState cs2 = r.createCullState();
        cs2.setCullFace(Face.Back);
        postSelectionMaterial[cs2.getType()] = cs2;
        
//        ZBufferState zs4 = r.createZBufferState();
//        zs4.setFunction(TestFunction.LessThan);
//        postSelectionMaterial[zs4.getType()] = zs4;
        
        // -- Placement Material --
        CullState cs3 = r.createCullState();
        cs3.setCullFace(Face.Back);
        placementMaterial[cs3.getType()] = cs3;
        
        ZBufferState zs2 = r.createZBufferState();
        zs2.setFunction(TestFunction.Always);
        zs2.setWritable(true);
        placementMaterial[zs2.getType()] = zs2;
        
        BlendState bl = r.createBlendState();
        bl.setBlendEnabled(true);
        bl.setConstantColor(new ColorRGBA(1.0f, 1.0f, 1.0f, 0.7f));
        bl.setSourceFunction(SourceFunction.SourceAlpha);
        bl.setDestinationFunction(DestinationFunction.ConstantAlpha);
        placementMaterial[bl.getType()] = bl;
        
        // -- Init brush lines --
        brushLines.setLineWidth(2.0f);
        brushLines.setAntialiased(true);
        brushLines.setMode(Mode.Loop);
        brushLines.setDefaultColor(ColorRGBA.green);
        
        // -- Brush states --
        ZBufferState zs3 = r.createZBufferState();
        zs3.setFunction(TestFunction.LessThan);
        zs3.setWritable(true);
        linesMaterial[zs3.getType()] = zs3;
    }
    
    private final RenderState[] savedStates2 = new RenderState[RenderState.RS_MAX_STATE];
    
    
    public void saveState(JmeContext cx){
        for (int i = 0; i < RenderState.RS_MAX_STATE; i++){
            savedStates2[i] = cx.getRenderContext().enforcedStateList[i];
        }
    }
    
    public void loadState(JmeContext cx){
        for (int i = 0; i < RenderState.RS_MAX_STATE; i++){
            cx.getRenderContext().enforcedStateList[i] = savedStates2[i];
        }
    }
    
    public void setDefaultMaterial(JmeContext cx){
        for (int i = 0; i < RenderState.RS_MAX_STATE; i++){
            if (Renderer.defaultStateList[i] != null)
                cx.getRenderContext().enforcedStateList[i] = Renderer.defaultStateList[i];
        }
    }
    
    public void clearMaterial(JmeContext cx){
        for (int i = 0; i < RenderState.RS_MAX_STATE; i++){
            cx.getRenderContext().enforcedStateList[i] = null;
        }
    }
    
    public void applyMaterial(JmeContext cx, RenderState[] mat){
        for (int i = 0; i < RenderState.RS_MAX_STATE; i++){
            if (mat[i] != null) cx.getRenderContext().enforcedStateList[i] = mat[i];
        }
    }
    
    public void resetMaterialForState(JmeContext cx, int renderState){
        cx.getRenderContext().enforcedStateList[renderState] = null;
    }
    
    @Override
    public void doRender(JmeContext cx){
        EditorState state = EditorState.getState();
        
        saveState(cx);
        
        List<Entity> entities = state.selection;
        if (entities.size() > 0){
             // Prepeare for selection rendering
            setDefaultMaterial(cx);
            applyMaterial(cx, selectionMaterial);
            
            for (Entity ent: entities){
                Spatial model = ent.getUnit(ModelUnit.class).getModel();
                model.setCullHint(CullHint.Dynamic);
                cx.getRenderer().draw(model);
                model.setCullHint(CullHint.Always);
            }
            cx.getRenderer().renderQueue();
            
            clearMaterial(cx);
            applyMaterial(cx, postSelectionMaterial);
            
            for (Entity ent: entities){
                Spatial model = ent.getUnit(ModelUnit.class).getModel();
                model.setCullHint(CullHint.Dynamic);
                cx.getRenderer().draw(model);
                model.setCullHint(CullHint.Always);
            }
        }
        
        
        if (state.editType == EditType.ENTITY && state.entityTypePrototype != null && !state.selectionMode){
            // Prepeare for placement rendering
            setDefaultMaterial(cx);
            applyMaterial(cx, placementMaterial);
            resetMaterialForState(cx, RenderState.RS_TEXTURE);

            Spatial s = state.entityTypePrototype.getUnit(ModelUnit.class).getModel();
            if (EditorState.lastMouseEvent.getID() != MouseEvent.MOUSE_EXITED){
                // mouse is on screen
                PickUtils.findClickedObject(EditorState.lastMouseEvent.getX(),
                                            EditorState.lastMouseEvent.getY(), 
                                            true, 
                                            point);
                s.setLocalTranslation(point);
                s.updateGeometricState(0, true);
                s.updateRenderState();
                s.draw(cx.getRenderer());
            }
        }else if (state.editType == EditType.TERRAIN || state.editType == EditType.TEXTURE){
            if (state.brushSize != brushRadius){
                brushLines.appendCircle(state.brushSize, 0.0f, 0.0f, 20, false);
            }
            
            PickUtils.findClickedObject(EditorState.lastMouseEvent.getX(),
                                            EditorState.lastMouseEvent.getY(), 
                                            true, 
                                            point);
            brushLines.setLocalTranslation(point.clone());
            brushLines.getLocalTranslation().y = 0.0f;
            brushLines.updateWorldVectors();
            
            FloatBuffer vb = brushLines.getVertexBuffer();
            vb.rewind();
            
            float angle = 0.0f;
            float step = FastMath.TWO_PI / 20.0f;
            float radius = state.brushSize;
            
            for (int i = 0; i < vb.limit() / 3; i++){
                point.set(vb.get(), vb.get(), vb.get());
                point.set(FastMath.cos(angle) * radius, 0.0f, FastMath.sin(angle) * radius);
                
                // convert to world space
                point.addLocal(brushLines.getLocalTranslation());
                
                // compute height
                float h = PickUtils.getTerrainHeight(World.getWorld(), point, null);
                point.y = h + 1.0f;
                
                // convert to local space
                point.subtractLocal(brushLines.getLocalTranslation());
                
                // write to buffer
                vb.position(vb.position()-3);
                vb.put(point.x).put(point.y).put(point.z);
                
                angle += step;
            }
            vb.rewind();
            brushLines.updateGeometricState(0, true);
            
            setDefaultMaterial(cx);
            applyMaterial(cx, linesMaterial);
            
            cx.getRenderer().draw(brushLines);
        }
        
        cx.getRenderer().renderQueue();
        
        loadState(cx);
    }
    
}
