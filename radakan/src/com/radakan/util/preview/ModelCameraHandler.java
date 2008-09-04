package com.radakan.util.preview;

import com.gibbon.jme.context.JmeContext;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import java.awt.event.InputEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.util.concurrent.Callable;

public class ModelCameraHandler extends MouseAdapter implements MouseMotionListener,
            MouseWheelListener {
    
    java.awt.Point last = new java.awt.Point(0, 0);
    Vector3f focus = new Vector3f();
    private Vector3f vector = new Vector3f();
    private Quaternion rot = new Quaternion();

    private final Camera cam;

    public ModelCameraHandler(Camera cam){
        this.cam=cam;
    }

    public void mouseDragged(final MouseEvent arg0) {
        Callable<?> exe = new Callable() {
            public Object call() {
                int difX = last.x - arg0.getX();
                int difY = last.y - arg0.getY();
                int mult = arg0.isShiftDown() ? 10 : 1;
                last.x = arg0.getX();
                last.y = arg0.getY();

                int mods = arg0.getModifiers();
                if ((mods & InputEvent.BUTTON1_MASK) != 0) {
                    rotateCamera(Vector3f.UNIT_Y, difX * 0.0025f);
                    rotateCamera(cam.getLeft(), -difY * 0.0025f);
                }
                if ((mods & InputEvent.BUTTON2_MASK) != 0 && difY != 0) {
                    zoomCamera(difY * mult);
                }
                if ((mods & InputEvent.BUTTON3_MASK) != 0) {
                    panCamera(-difX, -difY);
                }
                return null;
            }
        };
        JmeContext.get().executeLater(exe);
    }
    
    public void mouseMoved(MouseEvent arg0) {
    }
    
    @Override
    public void mousePressed(MouseEvent arg0) {
        last.x = arg0.getX();
        last.y = arg0.getY();
    }
    
    public void mouseWheelMoved(final MouseWheelEvent arg0) {
        Callable<?> exe = new Callable() {
            public Object call() {
                zoomCamera(arg0.getWheelRotation()
                        * (arg0.isShiftDown() ? -1 : -20));
                return null;
            }
        };
        JmeContext.get().executeLater(exe);
    }

    public void recenterCamera() {
        Callable<?> exe = new Callable() {
            public Object call() {
                Vector3f.ZERO.subtract(focus, vector);
                cam.getLocation().addLocal(vector);
                focus.addLocal(vector);
                cam.onFrameChange();
                return null;
            }
        };
        
        JmeContext.get().executeLater(exe);
    }
    
    private void rotateCamera(Vector3f axis, float amount) {
        if (axis.equals(cam.getLeft())) {
            float elevation = -FastMath.asin(cam.getDirection().y);
            amount = Math.min(Math.max(elevation + amount,
                    -FastMath.HALF_PI), FastMath.HALF_PI)
                    - elevation;
        }
        rot.fromAngleAxis(amount, axis);
        cam.getLocation().subtract(focus, vector);
        rot.mult(vector, vector);
        focus.add(vector, cam.getLocation());
        rot.mult(cam.getLeft(), cam.getLeft());
        rot.mult(cam.getUp(), cam.getUp());
        rot.mult(cam.getDirection(), cam.getDirection());
        cam.normalize();
        cam.onFrameChange();
    }
    
    private void panCamera(float left, float up) {
        cam.getLeft().mult(left, vector);
        vector.scaleAdd(up, cam.getUp(), vector);
        cam.getLocation().addLocal(vector);
        focus.addLocal(vector);
        cam.onFrameChange();
    }
    
    private void zoomCamera(float amount) {
        float dist = cam.getLocation().distance(focus);
        amount = dist - Math.max(0f, dist - amount);
        cam.getLocation().scaleAdd(amount, cam.getDirection(),
                cam.getLocation());
        cam.onFrameChange();
    }
}
