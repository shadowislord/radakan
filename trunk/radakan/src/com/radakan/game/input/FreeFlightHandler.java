package com.radakan.game.input;

import com.jme.math.Matrix3f;
import com.jme.renderer.Camera;

public class FreeFlightHandler implements IGameActionListener {

	private IInputMapper mapper = new DefaultInputMapper();
	
	private Camera cam;
	
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
		
		if(action.name.equals(InputActions.CAMERA_ROTATE_LEFT)) {
			incr.fromAngleNormalAxis(-action.velocity * .01f, cam.getUp());
			incr.mult(cam.getUp(), cam.getUp());
	        incr.mult(cam.getLeft(), cam.getLeft());
	        incr.mult(cam.getDirection(), cam.getDirection());
	        cam.normalize();
	        cam.update();
	        
	        return true;
		} else if (action.name.equals(InputActions.CAMERA_ROTATE_RIGHT)) {
			incr.fromAngleNormalAxis(action.velocity * .01f, cam.getUp());
			incr.mult(cam.getUp(), cam.getUp());
	        incr.mult(cam.getLeft(), cam.getLeft());
	        incr.mult(cam.getDirection(), cam.getDirection());
	        cam.normalize();
	        cam.update();
	        
			return true;
		}
		
		return false;
	}

}
