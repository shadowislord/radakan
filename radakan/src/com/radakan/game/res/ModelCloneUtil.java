package com.radakan.game.res;



import com.jme.scene.Controller;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import com.jme.scene.state.RenderState;
import com.jme.scene.state.RenderState;

public class ModelCloneUtil {

    public static Controller clone(Controller c){
        return null;
    }
    
    public static void cloneSpatial(Spatial source, Spatial target){
        target.setName(source.getName());
//        for (Controller c : source.getControllers()){
//            target.addController(clone(c));
//        }
        
        for (int i = 0; i < RenderState.RS_MAX_STATE; i++){
            target.setRenderState(source.getRenderState(i));
        }
        
        target.setIsCollidable(target.isCollidable());
        
        target.setLocalTranslation(source.getLocalTranslation().clone());
        target.setLocalRotation(source.getLocalRotation().clone());
        target.setLocalScale(source.getLocalScale().clone());
        
        target.setLocks(source.getLocks());
        target.setZOrder(source.getZOrder());
        
        target.setCullHint(source.getCullHint());
        target.setTextureCombineMode(source.getTextureCombineMode());
        target.setLightCombineMode(source.getLightCombineMode());
        target.setRenderQueueMode(source.getRenderQueueMode());
        target.setNormalsMode(source.getNormalsMode());
        target.setModelBound(source.getWorldBound());
        
    }
    
    public static void cloneMesh(TriMesh source, TriMesh target){
        cloneSpatial(source, target);
        
        // buffers
        target.setVertexBuffer(source.getVertexBuffer());
        target.setColorBuffer(source.getColorBuffer());
        target.setBinormalBuffer(source.getBinormalBuffer());
        target.setTangentBuffer(source.getTangentBuffer());
        target.setNormalBuffer(source.getNormalBuffer());
        target.setTextureCoords(source.getTextureCoords());
        
        // misc data
        target.setVertexCount(source.getVertexCount());
        target.setCastsShadows(source.isCastsShadows());
        target.setHasDirtyVertices(source.hasDirtyVertices());
        target.setDefaultColor(source.getDefaultColor());
        target.setDisplayListID(source.getDisplayListID());
        target.setVBOInfo(source.getVBOInfo());
        
        // trimesh specific
        target.setMode(source.getMode());
        target.setIndexBuffer(source.getIndexBuffer());
        
        // update from model data
        target.updateModelBound();
    }
    
    public static Spatial cloneSmart(Spatial source){
        if (source instanceof Node){
            Node cloned = new Node("temp");
            for (Spatial child : ((Node)source).getChildren()){
                Spatial clonedChild = cloneSmart(child);
                cloned.attachChild(clonedChild);
            }
            cloneSpatial(source, cloned);
            return cloned;
        }else if (source instanceof TriMesh){
            TriMesh cloned = new TriMesh("temp");
            cloneMesh((TriMesh)source, cloned);
            return cloned;
        }else{
            throw new UnsupportedOperationException();
        }
    }
    
}
