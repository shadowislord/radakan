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
