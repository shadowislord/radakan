package com.radakan.graphics.mesh.parser;

import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.VBOInfo;
import com.radakan.graphics.mesh.anim.MeshAnimationController;
import com.radakan.graphics.mesh.anim.OgreMesh;
import com.radakan.graphics.util.ModelCloneUtil;

public final class MeshCloner {

    public static final void setVBO(Node sourceMesh){
        for (Spatial child : sourceMesh.getChildren()){
            OgreMesh sourceSubmesh = (OgreMesh) child;
            VBOInfo info = new VBOInfo(true);
            
            // disable vertex and normal VBOs because those
            // will be updated every frame by the animation controller
            info.setVBOVertexEnabled(false);
            info.setVBONormalEnabled(false);
            info.setVBOIndexEnabled(false);
            
            sourceSubmesh.setVBOInfo(info);
        }
    }
    
    public static final void setLODLevel(Node sourceMesh, int level){
        for (Spatial child : sourceMesh.getChildren()){
            OgreMesh sourceSubmesh = (OgreMesh) child;
            sourceSubmesh.setLodLevel(level);
        }
    }
    
    public static final Node cloneMesh(Node sourceMesh){
        Node targetMesh = new Node(sourceMesh.getName());
        ModelCloneUtil.cloneSpatial(sourceMesh, targetMesh);
        
        OgreMesh[] submeshes = new OgreMesh[sourceMesh.getQuantity()];
        int index = 0;
        for (Spatial child : sourceMesh.getChildren()){
            OgreMesh sourceSubmesh = (OgreMesh) child;
            OgreMesh targetSubmesh = new OgreMesh(sourceSubmesh.getName());
            ModelCloneUtil.cloneMesh(sourceSubmesh, targetSubmesh);
            targetSubmesh.cloneFromMesh(sourceSubmesh);
            targetMesh.attachChild(targetSubmesh);
            submeshes[index] = targetSubmesh;
            index++;
        }
        
        MeshAnimationController sourceControl = (MeshAnimationController) sourceMesh.getController(0);
        MeshAnimationController targetControl = new MeshAnimationController(submeshes, sourceControl);
        
        targetMesh.addController(targetControl);
        
        return targetMesh;
    }
    
}
