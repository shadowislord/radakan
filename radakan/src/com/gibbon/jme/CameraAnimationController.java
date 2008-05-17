package com.gibbon.jme;

import com.jme.curve.Curve;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.scene.Controller;
import com.jme.scene.Spatial;

/**
 * @author Andreas Grabner
 */
public class CameraAnimationController extends Controller {

    private static final long serialVersionUID = 1L;
    private Spatial mover;
    private Curve curve;
    private float currentTime = 0.0f;
    private float deltaTime = 0.0f;
    private Curve upCurve;
    private Curve leftCurve;
    private Curve dirCurve;
    private Vector3f curUp,  curLeft,  curDir,  tmpVec;

    /**
     * Constructor instantiates a new <code>CameraAnimationController</code> object. The curve object that the
     * controller operates on and the spatial object that is moved is specified during construction.
     * 
     * @param curve the curve to operate on.
     * @param upCurve the curve with the up vectors.
     * @param leftCurve the curve with the left vectors.
     * @param dirCurve the curve with the dir vectors.
     * @param mover the spatial to move.
     */
    public CameraAnimationController(Curve curve, Curve upCurve, Curve leftCurve, Curve dirCurve, Spatial mover) {
        this.curve = curve;
        this.mover = mover;
        setMinTime(0);
        setMaxTime(10);
        setSpeed(1.0f);
        this.upCurve = upCurve;
        this.leftCurve = leftCurve;
        this.dirCurve = dirCurve;
        curUp = new Vector3f();
        curLeft = new Vector3f();
        curDir = new Vector3f();
    }

    /**
     * Constructor instantiates a new <code>CameraAnimationController</code> object. The curve object that the
     * controller operates on and the spatial object that is moved is specified during construction. The game time to
     * start and the game time to finish is also supplied.
     * 
     * @param curve the curve to operate on.
     * @param mover the spatial to move.
     * @param minTime the time to start the controller.
     * @param maxTime the time to end the controller.
     */
    public CameraAnimationController(Curve curve, Curve upCurve, Curve leftCurve, Curve dirCurve, Spatial mover,
            float minTime, float maxTime) {
        this.curve = curve;
        this.mover = mover;
        setMinTime(minTime);
        setMaxTime(maxTime);
        this.upCurve = upCurve;
        this.leftCurve = leftCurve;
        this.dirCurve = dirCurve;
        curUp = new Vector3f();
        curLeft = new Vector3f();
        curDir = new Vector3f();
    }

    /**
     * <code>update</code> moves a spatial along the given curve for along a time period.
     * 
     * @see com.jme.scene.Controller#update(float)
     */
    public void update(float time) {
        if (mover == null || curve == null || upCurve == null || leftCurve == null || dirCurve == null) {
            return;
        }

        currentTime += time * getSpeed();

        if (currentTime >= getMinTime() && currentTime <= getMaxTime()) {
            deltaTime = currentTime - getMinTime();
            Quaternion q = new Quaternion();

            upCurve.getPoint(deltaTime, curUp);
            leftCurve.getPoint(deltaTime, curLeft);
            dirCurve.getPoint(deltaTime, curDir);

            curUp.normalizeLocal();
            curLeft.normalizeLocal();
            curDir.normalizeLocal();

            tmpVec = curUp.cross(curDir);
            if (FastMath.acos(curLeft.dot(tmpVec)) * FastMath.RAD_TO_DEG > 90) {
                tmpVec.multLocal(-1);
            } // if
            curLeft = tmpVec;

            tmpVec = curUp.cross(curLeft);
            if (FastMath.acos(curDir.dot(tmpVec)) * FastMath.RAD_TO_DEG > 90) {
                tmpVec.multLocal(-1);
            } // if
            curDir = tmpVec;

            q.fromAxes(curLeft, curUp, curDir);
            mover.setLocalRotation(q);
            mover.setLocalTranslation(curve.getPoint(deltaTime, mover.getLocalTranslation()));
        }
    }
}
