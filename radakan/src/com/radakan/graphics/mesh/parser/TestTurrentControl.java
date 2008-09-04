package com.radakan.graphics.mesh.parser;

import com.jme.app.SimpleGame;
import com.jme.input.FirstPersonHandler;
import com.jme.input.KeyInput;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.scene.Spatial;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import com.radakan.graphics.mesh.anim.Bone;
import com.radakan.graphics.mesh.anim.MeshAnimationController;
import java.io.IOException;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;

public class TestTurrentControl extends SimpleGame {

    private Spatial model;
    private Bone turretBone;
    private float angle = 0f;
    private float angleVel = 0f;
    
    public static void main(String[] args){
        TestTurrentControl app = new TestTurrentControl();
        app.setConfigShowMode(ConfigShowMode.NeverShow);
        app.start();
    }
    
    @Override
    protected void simpleUpdate(){
        // acceleration
        if (KeyInput.get().isKeyDown(KeyInput.KEY_LEFT)){
            angleVel += tpf * 0.03f;
        }else if (KeyInput.get().isKeyDown(KeyInput.KEY_RIGHT)){
            angleVel -= tpf * 0.03f;
        }
        
        // drag
        if (angleVel > FastMath.ZERO_TOLERANCE){
            angleVel = Math.max(0f, angleVel - (tpf * 0.025f));
        }else if (angleVel < -FastMath.ZERO_TOLERANCE){
            angleVel = Math.min(0f, angleVel + (tpf * 0.025f));
        }
        
        // speed limit
        if (angleVel < -0.1f)
            angleVel = -0.1f;
        else if (angleVel > 0.1f)
            angleVel = 0.1f;
       
        // apply velocity
        angle += angleVel;
        
        Quaternion tempRot = new Quaternion();
        tempRot.fromAngleAxis(angle, Vector3f.UNIT_Y);
        turretBone.setUserTransforms(Vector3f.ZERO, tempRot, Vector3f.UNIT_XYZ);
    }
    
    protected void loadMeshModel(){
        OgreLoader loader = new OgreLoader();
        MaterialLoader matLoader = new MaterialLoader();
        
        try {
            SimpleResourceLocator srl = new SimpleResourceLocator(new URL("file:/D:/TestTurret/"));
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_MODEL, srl);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, srl);
            
            URL url2 = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, "Scene.material");
            URL url = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, "Cube.mesh.xml");
            
            
            if (url2 != null){
                matLoader.load(url2.openStream());
                if (matLoader.getMaterials().size() > 0)
                    loader.setMaterials(matLoader.getMaterials());
            }
            
            model = loader.loadModel(url);
            rootNode.attachChild(model);
        } catch (IOException ex) {
            Logger.getLogger(TestMeshLoading.class.getName()).log(Level.SEVERE, null, ex);
        } catch (URISyntaxException ex) {
            Logger.getLogger(TestMeshLoading.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        rootNode.updateGeometricState(0, true);
        rootNode.updateRenderState();
    }
    
    protected void setupTurretControl(){
        MeshAnimationController animControl = (MeshAnimationController) model.getController(0);
        animControl.setAnimation("Rotate");
        turretBone = animControl.getBone("Turret");
        turretBone.setUserControl(true);
    }
    
    @Override
    protected void simpleInitGame() {
        loadMeshModel();
        setupTurretControl();
        
        input.setEnabled(false);

        cam.setLocation(new Vector3f(5f, 5f, -6f));
        cam.lookAt(model.getWorldBound().getCenter(), Vector3f.UNIT_Y);
    }

    
}
