package com.radakan.game.gui;

import com.gibbon.jme.context.JmeContext;

import com.gibbon.jme.pass.RenderPass;
import com.jme.input.KeyInput;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.scene.Spatial;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;
import com.jme.system.DisplaySystem;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import com.radakan.game.Game;
import com.radakan.jme.mxml.SceneLoader;
import com.radakan.game.debug.GameDebugManager;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

public class StartScreenPass extends RenderPass {

    private float time = 0f;
    private static Vector3f[][] stageData;
    
    static {
        stageData = new Vector3f[][] { 
            { 
              new Vector3f(0.5905301f, 9.610628f, 1.5400668f),
              new Vector3f(0.0906688f, -5.1571988E-8f, 0.9958811f),
              new Vector3f(-0.284775f, 0.9582437f, 0.025927464f),
              new Vector3f(-0.9542971f, -0.28595096f, 0.086884946f)
            },
            {
              new Vector3f(-0.5782283f, 9.904903f, -0.56687295f),
              new Vector3f(0.5233645f, 9.313226E-10f, 0.852109f),
              new Vector3f(0.05959859f, 0.997551f, -0.036605395f),
              new Vector3f(-0.85002214f, 0.069943845f, 0.5220827f)
            },
            {
              new Vector3f(-0.5446168f, 9.896504f, 3.3903203f),
              new Vector3f(-0.37945345f, 6.9849193E-10f, 0.9252109f),
              new Vector3f(0.055478986f, 0.9982006f, 0.022753382f),
              new Vector3f(-0.92354596f, 0.059965007f, -0.37877053f)
            }
        };
    }
    
    public static final float interpolateCatmullRom(float v0, float v1, float v2, float v3, float x){
        return 0.5f * ((-v0 + 3f*v1 -3f*v2 + v3)*x*x*x
                    +  (2f*v0 -5f*v1 + 4f*v2 - v3)*x*x
                    +  (-v0+v2)*x
                    +  2f*v1);

    }
    
    public static final Vector3f interpolateCatmullRom(Vector3f v0, Vector3f v1, Vector3f v2, Vector3f v3, float x, Vector3f store){
        if (store == null)
            store = new Vector3f();
        
        store.x = interpolateCatmullRom(v0.x, v1.x, v2.x, v3.x, x);
        store.y = interpolateCatmullRom(v0.y, v1.y, v2.y, v3.y, x);
        store.z = interpolateCatmullRom(v0.z, v1.z, v2.z, v3.z, x);
        return store;
    }
    
    public static final Quaternion interpolateCatmullRom(Quaternion v0, Quaternion v1, Quaternion v2, Quaternion v3, float x, Quaternion store){
        if (store == null)
            store = new Quaternion();
        
        store.x = interpolateCatmullRom(v0.x, v1.x, v2.x, v3.x, x);
        store.y = interpolateCatmullRom(v0.y, v1.y, v2.y, v3.y, x);
        store.z = interpolateCatmullRom(v0.z, v1.z, v2.z, v3.z, x);
        store.w = interpolateCatmullRom(v0.w, v1.w, v2.w, v3.w, x);
        store.normalize();
        return store;
    }
    
    public StartScreenPass(){
        try {      
            SceneLoader loader = new SceneLoader();
            InputStream in = Game.getResource("start.scene").openStream();
            loader.load(in);
            in.close();

            Spatial scene = loader.getScene();
            
            ZBufferState zbuf = JmeContext.get().getRenderer().createZBufferState();
            zbuf.setFunction(TestFunction.LessThan);
            scene.setRenderState(zbuf);

//            CullState cull = JmeContext.get().getRenderer().createCullState();
//            cull.setCullFace(CullState.Face.Back);
//            scene.setRenderState(cull);
            
            scene.updateGeometricState(0, true);
            scene.updateRenderState();
            
            add(scene);
            //cam = loader.getCamera();
        } catch (IOException ex) {
            GameDebugManager.reportError("Error while loading start screen scene", ex);
        }
    }
    
    public void setTime(float time, Camera cam){
        int startStage = (int) FastMath.floor(time);
        int endStage = (int) FastMath.ceil(time);
        if (time < 0.0f){
            cam.setFrame(stageData[0][0], stageData[0][1], stageData[0][2], stageData[0][3]);
            return;
        }else if (time > stageData.length-1){
            endStage = stageData.length - 1;
            cam.setFrame(stageData[endStage][0], 
                         stageData[endStage][1], 
                         stageData[endStage][2],
                         stageData[endStage][3]);
            return;
        }
        
        int prevStage = Math.max(0, startStage - 1);
        int nextStage = Math.min(stageData.length - 1, endStage + 1);
        float timeDelta = time - FastMath.floor(time);
        
        Vector3f[] results = new Vector3f[4];
        for (int i = 0; i < results.length; i++){
            results[i] = interpolateCatmullRom(stageData[prevStage][i],
                                               stageData[startStage][i],
                                               stageData[endStage][i],
                                               stageData[nextStage][i],
                                               timeDelta,
                                               null);
            if (i != 0) results[i].normalizeLocal();
        }
        
        cam.getLocation().set(results[0]);
        cam.getLeft().set(results[1]);
        cam.getUp().set(results[2]);
        cam.getDirection().set(results[3]);
        
//        cam.getLocation().set(stageData[startStage][0])
//                .interpolate(stageData[endStage][0], time - FastMath.floor(time));
//        
//        cam.getLeft().set(stageData[startStage][1])
//                .interpolate(stageData[endStage][1], time - FastMath.floor(time));
//        cam.getLeft().normalizeLocal();
//        cam.getUp().set(stageData[startStage][2])
//                .interpolate(stageData[endStage][2], time - FastMath.floor(time));
//        cam.getUp().normalizeLocal();
//        cam.getDirection().set(stageData[startStage][3])
//                .interpolate(stageData[endStage][3], time - FastMath.floor(time));
//        cam.getDirection().normalizeLocal();
        
        cam.update();
    }
    
    @Override
    public void initPass(JmeContext cx){
        //cx.getRenderer().setCamera(cam);
        //cx.getRenderer().getCamera().setFrame(cam.getLocation(), cam.getLeft(), cam.getUp(), cam.getDirection());
        
        setTime(0.0f, cx.getRenderer().getCamera());
        //cx.getRenderer().getCamera().setFrame(stageData[0][0], left, up, dir);
//        cx.getRenderer().setBackgroundColor(ColorRGBA.darkGray);
    }
    
    @Override
    public void doUpdate(JmeContext cx){
        super.doUpdate(cx);
        
        time += cx.getPassManager().getTPF();
        
        float localTime = time / 30f;
        setTime(localTime, cx.getRenderer().getCamera());
        
        if (KeyInput.get().isKeyDown(KeyInput.KEY_C)){
            System.out.println("------------------");
            System.out.println(cx.getRenderer().getCamera().getLocation());
            System.out.println(cx.getRenderer().getCamera().getLeft());
            System.out.println(cx.getRenderer().getCamera().getUp());
            System.out.println(cx.getRenderer().getCamera().getDirection());
        }
    }
    
    public void doRender(JmeContext cx){
        super.doRender(cx);
    }
    
}
