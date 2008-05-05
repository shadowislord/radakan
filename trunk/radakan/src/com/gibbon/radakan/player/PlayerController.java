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

package com.gibbon.radakan.player;

import com.gibbon.jme.context.JmeContext;
import com.jme.bounding.BoundingBox;
import com.jme.input.ChaseCamera;
import com.jme.input.InputHandler;
import com.jme.input.ThirdPersonHandler;
import com.jme.intersection.PickData;
import com.jme.intersection.TriangleCollisionResults;
import com.jme.intersection.TrianglePickResults;
import com.jme.math.FastMath;
import com.jme.math.Ray;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.scene.Controller;
import com.jme.scene.Geometry;
import com.jme.scene.Node;
import com.jme.scene.TriMesh;
import com.model.md5.ModelNode;
import com.model.md5.controller.JointController;
import java.util.HashMap;
import java.util.List;
import java.util.concurrent.Callable;

public class PlayerController extends Controller {

    private ModelNode player;
    private Node walkableNode;
    private Camera cam;
    private ChaseCamera chaser = null;
    private ThirdPersonHandler input;
    private JointController animControl;
    
    private Ray camRay = new Ray(new Vector3f(), new Vector3f(0,-1,0));
    private float terrainHeight = Float.NaN;
    private float playerHeight;
    
    private Vector3f lastLoc = new Vector3f();
    private boolean isWalking = false;
    
    public PlayerController(final ModelNode player, Camera camera, Node walkableNode){
        this.player = player;
        this.cam = camera;
        this.walkableNode = walkableNode;
        camResults.setCheckDistance(true);
        
        playerHeight = ((BoundingBox)player.getWorldBound()).yExtent;
        lastLoc.set(player.getLocalTranslation());
        
        animControl = (JointController) player.getController(0);
        
        JmeContext.get().execute(new Callable<Void>(){
            public Void call(){
                Vector3f targetOffset = new Vector3f(0, playerHeight * 0.7f, 0);
                chaser = new ChaseCamera(cam, player);
                chaser.setTargetOffset(targetOffset);
                chaser.setMinDistance(playerHeight * 2.0f);
                chaser.setMaxDistance(playerHeight * 5.0f);

                HashMap<String, Object> handlerProps = new HashMap<String, Object>();
                handlerProps.put(ThirdPersonHandler.PROP_DOGRADUAL, "true");
                handlerProps.put(ThirdPersonHandler.PROP_TURNSPEED, ""+(0.7f * FastMath.PI));
                handlerProps.put(ThirdPersonHandler.PROP_LOCKBACKWARDS, "true");
                handlerProps.put(ThirdPersonHandler.PROP_CAMERAALIGNEDMOVE, "true");

                input = new ThirdPersonHandler(player, cam, handlerProps);
                input.setActionSpeed(1f);
                
                return null;
            }
        });
    }
    
    public InputHandler getHandler(){
        return input;
    }
    
    TrianglePickResults camResults = new TrianglePickResults() {
        @Override
        public void processPick() {
            if (getNumber() > 0) {
                // get the state of the triangle picking
                PickData pData = getPickData(0);

                // triangles that collided with the ray (e.g under the player)
                List tris = pData.getTargetTris();

                // mesh with which the collision was made
                TriMesh mesh = (TriMesh) pData.getTargetMesh();

                if (tris.isEmpty()) return;
                
                // get the first triangle under the player
                int triIndex = ((Integer) tris.get(0)).intValue();
                Vector3f[] vec = new Vector3f[3];
                mesh.getTriangle(triIndex, vec);
                
                // convert triangle to world space
                for (int x = 0; x < vec.length; x++) {
                    vec[x].multLocal(mesh.getWorldScale());
                    mesh.getWorldRotation().mult(vec[x], vec[x]);
                    vec[x].addLocal(mesh.getWorldTranslation());
                }

                // find WHERE the triangle was picked.. 
                Vector3f loc = new Vector3f();
                pData.getRay().intersectWhere(vec[0], vec[1], vec[2], loc);

                // height of the player/camera
                // if the player's feet are at the origin
                // this is 0
                terrainHeight = loc.y;
            }else{
                terrainHeight = Float.NaN;
            }
        }
    };
    
    TriangleCollisionResults colResults = new TriangleCollisionResults();
    
    @Override
    public void update(float tpf) {
        if (chaser == null)
            return;
        
        chaser.update(tpf);

//        camRay.getOrigin().set(cam.getLocation());
//        camResults.clear();
//        walkableNode.calculatePick(camRay, camResults);
//        
//        float camMinHeight = terrainHeight + 2f;
//        if (!Float.isInfinite(camMinHeight) && !Float.isNaN(camMinHeight)) {
//            cam.getLocation().y = camMinHeight;
//            cam.update();
//        }
        
        boolean moving = input.isWalkingBackwards() || input.isStrafing() || input.isWalkingForward();
        if (moving && !isWalking){
            animControl.setActiveAnimation("walk", .05f);
            animControl.setSpeed(1.5f);
            isWalking = true;
        }else if (!moving && isWalking){
            animControl.setActiveAnimation("stand", .5f);
            animControl.setSpeed(1f);
            isWalking = false;
        }
        
        boolean collided = false;
//        if (moving){
//            colResults.clear();
//            walkableNode.findCollisions(player, colResults);
//            for (int i = 0; i < colResults.getNumber(); i++){
//                CollisionData data = colResults.getCollisionData(i);
//                if (!data.getTargetMesh().getName().contains(".terrain")){
//                    player.setLocalTranslation(lastLoc);
//                    collided = true;
//                }
//            }
//        }
        
        if (!collided){
            camRay.getOrigin().set(player.getLocalTranslation());
            camRay.getOrigin().y += playerHeight;
            camResults.clear();
            walkableNode.calculatePick(camRay, camResults);

            float characterMinHeight = terrainHeight + playerHeight;
//            if (!Float.isInfinite(characterMinHeight) && !Float.isNaN(characterMinHeight) && ( Math.abs(characterMinHeight-lastLoc.y) < playerHeight) ) {
//                player.getLocalTranslation().y = characterMinHeight;
//                
//            }
            if (!Float.isInfinite(characterMinHeight) && !Float.isNaN(characterMinHeight) ) {
                player.getLocalTranslation().y = characterMinHeight;
                
            }
        }
        
        lastLoc.set(player.getLocalTranslation());
    }

}
