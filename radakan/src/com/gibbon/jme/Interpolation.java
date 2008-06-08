/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.gibbon.jme;

import com.jme.math.FastMath;

/**
 *
 * @author Kirill
 */
public class Interpolation {

    public static final float interpolateHermite(float v0, float v1, float v2, float v3, float x, float tension, float bias){
       float mu2 = x * x;
       float mu3 = mu2 * x;

       float m0, m1, a0, a1, a2, a3;
       
       m0  = (v1 - v0) * (1 + bias) * (1 - tension) / 2;
       m0 += (v2 - v1) * (1 - bias) * (1 - tension) / 2;
       m1  = (v2 - v1) * (1 + bias) * (1 - tension) / 2;
       m1 += (v3 - v2) * (1 - bias) * (1 - tension) / 2;

       a0  =  2 * mu3 - 3 * mu2 + 1;
       a1  =      mu3 - 2 * mu2 + x;
       a2  =      mu3 -     mu2;
       a3  = -2 * mu3 + 3 * mu2;

       return a0 * v1 
            + a1 * m0 
            + a2 * m1 
            + a3 * v2;
    }

    public static final float interpolateCubic(float v0, float v1, float v2, float v3, float x){
        float p = (v3 - v2) - (v0 - v1);
        float q = (v0 - v1) - p;
        float r = v2 - v0;
        float s = v1;

        return p * x * x * x 
             + q * x * x 
             + r * x 
             + s;
    }

    public static final float interpolateCosine(float v0, float v1, float x){
        float ft = x * FastMath.PI;
        float f = (1 - FastMath.cos(ft)) * 0.5f;

        return v0 * (1 - f) + v1 * f;
    }

    public static final float interpolateLinear(float v0, float v1, float x){
        return v0 * (1 - x) + v1 * x;
    }

    public static final float noise1(int x){
       x = (x << 13) ^ x;
       x = (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff;
       return 1.0f - x / 1073741824.0f;
    }

    public static final float noiseSmooth1(int x){
       return noise1(x) / 2.0f + noise1(x-1) / 4.0f + noise1(x+1) / 4.0f;
    }
    
}
