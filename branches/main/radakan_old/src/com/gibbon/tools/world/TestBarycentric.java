package com.gibbon.tools.world;

import com.jme.math.Ray;
import com.jme.math.Vector3f;

public class TestBarycentric {

    public static void main(String[] args){
        Ray r = new Ray(new Vector3f(0, 1, 0), Vector3f.UNIT_Y.negate());
        
        Vector3f v1 = new Vector3f(-0.5f, 0, -1);
        Vector3f v2 = new Vector3f(0.5f,  0, -1);
        Vector3f v3 = new Vector3f(0,     0,  1);
        
        Vector3f point = new Vector3f();
        r.intersectWherePlanar(v1, v2, v3, point);
        
        Vector3f diff1 = v2.subtract(v1);
        Vector3f diff2 = v3.subtract(v1);
        
        diff1.multLocal(point.y);
        diff2.multLocal(point.z);
        
        v1.addLocal(diff1).addLocal(diff2);
        
        System.out.println(v1);
    }
    
    /* OUTPUT: 
     * com.jme.math.Vector3f [X=0.0, Y=0.0, Z=0.0]
     */
}
