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

package com.radakan.entity.unit;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.pass.InputPass;
import com.gibbon.jme.pass.PassManager;
import com.jme.bounding.BoundingBox;
import com.jme.input.ChaseCamera;
import com.jme.input.InputHandler;
import com.jme.input.ThirdPersonHandler;
import com.jme.math.FastMath;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.scene.Spatial;
import com.jme.system.DisplaySystem;
import java.io.IOException;
import java.io.PrintStream;

import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.radakan.entity.Entity;
import com.radakan.util.XMLUtil;
import java.util.HashMap;
import org.w3c.dom.Node;

public class PlayerBehaviorUnit extends Unit {

    private Camera cam;
    
    private ChaseCamera chaser = null;
    private ThirdPersonHandler input;
    
    private boolean isWalking = false;
    private float modelHeight = -1f;
    private Spatial model;
    
    public PlayerBehaviorUnit(){
    }
    
    @Override
    public void attach(Entity ent){
        super.attach(ent);
        
//        if (!ent.hasUnits(MODEL | ANIMATION))
//            throw new RuntimeException("MODEL and ANIMATION units required!");
        
        // XXX: Assumption that bound volume is a box
        cam = DisplaySystem.getDisplaySystem().getRenderer().getCamera();
        model = entity.getModel().getModel();
        modelHeight = ((BoundingBox)model.getWorldBound()).yExtent;
        setupCamera(model);
    }
    
    @Override
    public void detach(){
        super.detach();
        
        chaser.setEnabled(false);
        input.setEnabled(false);
    }
    
    private void setupCamera(Spatial model){
        // XXX: Maybe create a CameraLockUnit for this?
        Vector3f targetOffset = new Vector3f(0, modelHeight * 0.7f, 0);
        chaser = new ChaseCamera(cam, model);
        chaser.setTargetOffset(targetOffset);
        chaser.setMinDistance(modelHeight * 2.0f);
        chaser.setMaxDistance(modelHeight * 5.0f);

        HashMap<String, Object> handlerProps = new HashMap<String, Object>();
        handlerProps.put(ThirdPersonHandler.PROP_DOGRADUAL, "true");
        handlerProps.put(ThirdPersonHandler.PROP_TURNSPEED, ""+(0.7f * FastMath.PI));
        handlerProps.put(ThirdPersonHandler.PROP_LOCKBACKWARDS, "true");
        handlerProps.put(ThirdPersonHandler.PROP_CAMERAALIGNEDMOVE, "true");
        
        input = new ThirdPersonHandler(model, cam, handlerProps);
        input.setActionSpeed(10f);
        input.setEnabled(true);

        // XXX: Assumption that JmeContext is used
        PassManager pManager = JmeContext.get().getPassManager();
        InputPass iPass = (InputPass) pManager.getPass("Input");
        if (iPass == null)
            throw new IllegalStateException("Cannot locate InputPass in the PassManager");
        
        // Allows ThirdPersonHandler to recieve input from the InputPass
        if (iPass.getInputHandler() == null)
            iPass.setInputHandler(new InputHandler());
        
        iPass.getInputHandler().addToAttachedHandlers(input);
    }
    
    @Override
    public void update(float tpf) {
        chaser.update(tpf);
        
        IAnimationUnit anim = entity.getUnit(IAnimationUnit.class);
        boolean moving = input.isWalkingBackwards() || input.isStrafing() || input.isWalkingForward();
        if (moving && !isWalking){
            anim.play("walk");
            anim.setSpeed(2.0f);
            isWalking = true;
        }else if (!moving && isWalking){
            anim.play("stand");
            anim.setSpeed(0.5f);
            isWalking = false;
        }
        
        
        
        //boolean collided = false;
        //if (!collided){
          //  model.updateWorldVectors();
            //float terrainHeight = TileManager.getInstance().getTerrainHeight(model.getWorldTranslation());
            //float characterMinHeight = terrainHeight + modelHeight;
//            if (!Float.isInfinite(characterMinHeight) && !Float.isNaN(characterMinHeight) && ( Math.abs(characterMinHeight-lastLoc.y) < playerHeight) ) {
//                player.getLocalTranslation().y = characterMinHeight;
//                
//            }
            //if (!Float.isInfinite(characterMinHeight) && !Float.isNaN(characterMinHeight) ) {
            //    model.getLocalTranslation().y = characterMinHeight;
            //}
        //}
        
        //model.updateGeometricState(tpf, true);
        
        //lastLoc.set(player.getLocalTranslation());
    }

    public int getType() {
        return BEHAVIOR;
    }

    public void onUnitEvent(UnitEvent event) {
    }
        
    @Override
    public void write(JMEExporter ex) throws IOException {
    }
    
    @Override
    public void read(JMEImporter im) throws IOException {
    }

    public void exportXML(PrintStream stream) {
    }

    public void importXML(Node rootBehaviorNode) {
        if (XMLUtil.getBoolAttribute(rootBehaviorNode, "default", false)){
            // load default values..
        }
    }
}
