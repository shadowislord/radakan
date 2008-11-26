package com.radakan.game.input;

import com.jme.math.Matrix3f;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;

public class FreeFlightHandler implements IGameActionListener {
	
	private static final Vector3f tempVa = new Vector3f();

	private IInputMapper mapper = new DefaultInputMapper();
	
	private Camera cam;
	private float speed = 2.0f;
	
	public FreeFlightHandler(Camera camera) {
		this.cam = camera;
	}
	
	@Override
	public IInputMapper getMapper() {
		return mapper;
	}

	@Override
	public boolean onActionTriggered(InputActionEvent action) {
		Matrix3f incr = new Matrix3f();
		
		if(action.name.equals(InputActions.CAMERA_ROTATE_RIGHT) || action.name.equals(InputActions.PLAYER_TURN_RIGHT)) {
			incr.fromAngleNormalAxis(-action.velocity * .01f, cam.getUp());
			incr.mult(cam.getUp(), cam.getUp());
	        incr.mult(cam.getLeft(), cam.getLeft());
	        incr.mult(cam.getDirection(), cam.getDirection());
	        cam.normalize();
	        cam.update();
	        
	        return true;
		} else if (action.name.equals(InputActions.CAMERA_ROTATE_LEFT) || action.name.equals(InputActions.PLAYER_TURN_LEFT)) {
			incr.fromAngleNormalAxis(action.velocity * .01f, cam.getUp());
			incr.mult(cam.getUp(), cam.getUp());
	        incr.mult(cam.getLeft(), cam.getLeft());
	        incr.mult(cam.getDirection(), cam.getDirection());
	        cam.normalize();
	        cam.update();
	        
			return true;
		} else if(action.name.equals(InputActions.CAMERA_ROTATE_UP)) {
			incr.fromAngleNormalAxis(-action.velocity * .01f, cam.getLeft());
	        incr.mult(cam.getLeft(), cam.getLeft());
	        incr.mult(cam.getUp(), cam.getUp());
	        incr.mult(cam.getDirection(), cam.getDirection());
	        cam.normalize();
	        cam.update();
	        
	        return true;
		} else if (action.name.equals(InputActions.CAMERA_ROTATE_DOWN)) {
			incr.fromAngleNormalAxis(action.velocity * .01f, cam.getLeft());
	        incr.mult(cam.getLeft(), cam.getLeft());
	        incr.mult(cam.getUp(), cam.getUp());
	        incr.mult(cam.getDirection(), cam.getDirection());
	        cam.normalize();
	        cam.update();
	        
			return true;
		} else if(action.name.equals(InputActions.PLAYER_MOVE_BACK)) {
			Vector3f loc = cam.getLocation();
	        if ( !cam.isParallelProjection() ) {
	            loc.addLocal(cam.getDirection().mult(action.velocity * speed, tempVa));
	        } else {
	            // move up instead of forward if in parallel mode
	            loc.addLocal(cam.getUp().mult(action.velocity * speed, tempVa));
	        }
	        cam.update();
	        
	        return true;
		} else if (action.name.equals(InputActions.PLAYER_MOVE_FWD)) {
			Vector3f loc = cam.getLocation();
	        if ( !cam.isParallelProjection() ) {
	            loc.addLocal(cam.getDirection().mult(-action.velocity * speed, tempVa));
	        } else {
	            // move up instead of forward if in parallel mode
	            loc.addLocal(cam.getUp().mult(-action.velocity * speed, tempVa));
	        }
	        cam.update();
	        
	        return true;
		}
		
		return false;
	}

}
