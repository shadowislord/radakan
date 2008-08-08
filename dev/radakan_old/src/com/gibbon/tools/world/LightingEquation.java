package com.gibbon.tools.world;

import com.jme.math.Vector3f;
import com.jme.scene.state.LightState;
import com.jme.scene.state.MaterialState;

public class LightingEquation {

//    public static void lighting(MaterialState material, LightState lighting, Vector3f pos, Vector3f normal){
//        result.set(ColorRGBA.black);
////                result.set( (normal.x + 1.0f) / 2.0f, 
////                            (normal.y + 1.0f) / 2.0f, 
////                            (normal.z + 1.0f) / 2.0f, 
////                             1.0f);
////                result.set(point.y / 20f, point.y / 20f, point.y / 20f, 1.0f);
////                result.clamp();
//
//        for (Light light : lighting.getLightList()){
//            // C = (L . N) * Nd + Na
//
//            diffuse.set(ColorRGBA.black);
//            ambient.set(ColorRGBA.black);
//
//            diffuse.set(material.getDiffuse());
//            diffuse.r *= light.getDiffuse().r;
//            diffuse.g *= light.getDiffuse().g;
//            diffuse.b *= light.getDiffuse().b;
//            diffuse.a *= light.getDiffuse().a;
//
//            ambient.set(material.getAmbient());
//            ambient.r *= light.getAmbient().r;
//            ambient.g *= light.getAmbient().g;
//            ambient.b *= light.getAmbient().b;
//            ambient.a *= light.getAmbient().a;
//
//            /*if (light instanceof DirectionalLight){
//                DirectionalLight dl = (DirectionalLight) light;
//                float NdotL = dl.getDirection().dot(normal);
//
//                Ray r = new Ray(point, dl.getDirection());
//                TriMesh collided = (TriMesh) PickUtils.calculatePick(World.getWorld(), r, tempV, false);
//
//                // temp = Nd * NdotL + Na
//                if (collided != null){
//                    tempC.set(diffuse).multLocal(NdotL).addLocal(ambient);
//                }else{
//                    tempC.set(ambient);
//                }
//                tempC.clamp();
//                result.addLocal(tempC);*/
//            if (light instanceof PointLight){
//                PointLight pl = (PointLight) light;
//                // tempV = normalize(light_pos - point)
//                tempV.set(pl.getLocation()).subtractLocal(point).normalizeLocal();
//
//                Ray r = new Ray(point.clone(), tempV.clone());
//                float dist = PickUtils.distanceToGeometry(r, false);
//
//                float NdotL = tempV.dot(normal);
//                // tempC = Nd * NdotL + Na
//                if (NdotL < 0.0f){
//                    tempC.set(ambient);
//                }else{
//                    if (!Float.isNaN(dist)){
//                        tempC.set(ambient);
//                    }else{
//                        tempC.set(diffuse).multLocal(NdotL).addLocal(ambient);
//                    }
//                }
//
//                tempC.clamp();
//                result.addLocal(tempC);
//            }
//        }
//
//        result.clamp();
//    }
    
}
