package com.gibbon.jme;

import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.scene.Controller;
import com.jme.scene.Spatial;

public class LookAtController extends Controller {

    private Vector3f up;
    private Vector3f target;
    private Spatial targetSpatial;
    private Spatial source;
    
    private Vector3f tmpV = new Vector3f();
    private Quaternion tmpQ = new Quaternion();
    
    public LookAtController(Spatial source, Vector3f target){
        this.source = source;
        this.target = target;
        up = Vector3f.UNIT_Y;
    }
    
    public LookAtController(Spatial source, Spatial target){
        this.source = source;
        this.targetSpatial = target;
        up = Vector3f.UNIT_Y;
    }
    
    public void setUpVector(Vector3f up){
        this.up = up;
    }
    
    public void update(float tpf) {
        if (!isActive())
            return;
        
        if (targetSpatial != null){
            targetSpatial.updateWorldVectors();
            target = targetSpatial.getWorldTranslation();
        }
        
        // get the direction vector
        tmpV.set(target).subtractLocal(source.getWorldTranslation());
        
        // convert to a quaternion
        tmpQ.lookAt(target, up);
        
        // interpolate source's orientation toward the direction
        source.getLocalRotation().slerp(tmpQ, tpf * getSpeed());
    }

}
