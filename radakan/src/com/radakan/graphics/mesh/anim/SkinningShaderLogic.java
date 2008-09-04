package com.radakan.graphics.mesh.anim;

import com.jme.scene.Geometry;
import com.jme.scene.state.GLSLShaderDataLogic;
import com.jme.scene.state.GLSLShaderObjectsState;

public class SkinningShaderLogic implements GLSLShaderDataLogic {
    public void applyData(GLSLShaderObjectsState shader, Geometry geom) {
        MeshAnimationController c = (MeshAnimationController) geom.getParent().getController(0);
        
        c.getSkeleton().sendToShader(shader);
        ((OgreMesh) geom).getWeightBuffer().sendToShader(shader);
    }
}
