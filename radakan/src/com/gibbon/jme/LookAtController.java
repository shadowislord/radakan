/*
 * Radakan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.gibbon.jme;

import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.scene.Controller;
import com.jme.scene.Spatial;

/**
 * Makes the spatial always look at a certain target
 * (a Spatial or a Vector3)
 * 
 * @author Momoko_Fan
 */
public class LookAtController extends Controller {

    private Vector3f up;
    private Vector3f target;
    private Spatial targetSpatial;
    private Spatial source;
    
    private Vector3f tmpV = new Vector3f();
    private Quaternion tmpQ = new Quaternion();
    
    /**
     * Creates a new LookAtController that will always face <code>target</code>
     * 
     * @param source The spatial to control
     * @param target The target to face
     */
    public LookAtController(Spatial source, Vector3f target){
        this.source = source;
        this.target = target;
        up = Vector3f.UNIT_Y;
    }
    
    /**
     * Creates a new LookAtController that will always face <code>target</code><br/>
     * Since the target is a spatial, the lookat position could change over time.
     * 
     * @param source The spatial to control
     * @param target The target to face
     */
    public LookAtController(Spatial source, Spatial target){
        this.source = source;
        this.targetSpatial = target;
        up = Vector3f.UNIT_Y;
    }
    
    /**
     * Sets the UP vector of the source spatial for use in facing
     * 
     * @param up The UP vector
     */
    public void setUpVector(Vector3f up){
        this.up = up;
    }
    
    /**
     * Updates this controller, only used internally by jME code.
     * 
     * @param tpf Time per frame (1 / fps)
     */
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
