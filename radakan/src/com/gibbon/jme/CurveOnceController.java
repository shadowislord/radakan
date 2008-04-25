package com.gibbon.jme;

import com.jme.curve.Curve;
import com.jme.math.Vector3f;
import com.jme.scene.Controller;
import com.jme.scene.Spatial;
import com.jme.util.export.InputCapsule;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.jme.util.export.OutputCapsule;
import java.io.IOException;

public class CurveOnceController extends Controller {

    private static final long serialVersionUID = 1L;
    private Spatial mover;
    private Curve curve;
    private Vector3f up;
    private float orientationPrecision = 0.1f;
    private float currentTime = 0.0f;
    private float deltaTime = 0.0f;
    private boolean cycleForward = true;
    private boolean autoRotation = false;
    private Vector3f newPoint = new Vector3f();     // ***** added
    private Vector3f lastPoint = new Vector3f();    // ***** added 
    private boolean disableAfter;                   // ***** added

    public CurveOnceController(Curve curve, Spatial mover) {
        this.curve = curve;
        this.mover = mover;
        setUpVector(new Vector3f(0, 1, 0));
        setMinTime(0);
        setMaxTime(Float.MAX_VALUE);
        setRepeatType(Controller.RT_CLAMP);
        setSpeed(1.0f);
    }

    public CurveOnceController(Curve curve, Spatial mover, float minTime, float maxTime){
        this.curve = curve;
        this.mover = mover;
        setMinTime(minTime);
        setMaxTime(maxTime);
        setRepeatType(Controller.RT_CLAMP);
    }

    public void setUpVector(Vector3f up) {
        this.up = up;
    }

    public void setOrientationPrecision(float value) {
        orientationPrecision = value;
    }

    public void setAutoRotation(boolean value) {
        autoRotation = value;
    }

    public boolean isAutoRotating() {
        return autoRotation;
    }

    public void update(float time) {
        if (mover == null || curve == null || up == null) {
            return;
        }
        currentTime += time * getSpeed();

        if (currentTime >= getMinTime() && currentTime <= getMaxTime()) {
            if (getRepeatType() == RT_CLAMP) {
                deltaTime = currentTime - getMinTime();
                newPoint = curve.getPoint(deltaTime, mover.getLocalTranslation());   // ***** added
                mover.setLocalTranslation(newPoint);   // ***** added
                if (autoRotation) {   // ***** added
                    mover.setLocalRotation(
                            curve.getOrientation(
                            deltaTime,
                            orientationPrecision,
                            up));
                }
                if (isDisableAfterClamp() && lastPoint != null) {
                    if (lastPoint.equals(newPoint)) {
                        setActive(false);
                        lastPoint.set(0, 0, 0);
                        currentTime = 0.0f;
                    }
                    lastPoint.set(newPoint);
                }
            } else if (getRepeatType() == RT_WRAP) {
                deltaTime = (currentTime - getMinTime()) % 1.0f;
                if (deltaTime > 1) {
                    currentTime = 0;
                    deltaTime = 0;
                }
                mover.setLocalTranslation(curve.getPoint(deltaTime, mover.getLocalTranslation()));
                if (autoRotation) {
                    mover.setLocalRotation(
                            curve.getOrientation(
                            deltaTime,
                            orientationPrecision,
                            up));
                }
            } else if (getRepeatType() == RT_CYCLE) {
                float prevTime = deltaTime;
                deltaTime = (currentTime - getMinTime()) % 1.0f;
                if (prevTime > deltaTime) {
                    cycleForward = !cycleForward;
                }
                if (cycleForward) {
                    mover.setLocalTranslation(curve.getPoint(deltaTime, mover.getLocalTranslation()));
                    if (autoRotation) {
                        mover.setLocalRotation(
                                curve.getOrientation(
                                deltaTime,
                                orientationPrecision,
                                up));
                    }
                } else {
                    mover.setLocalTranslation(
                            curve.getPoint(1.0f - deltaTime, mover.getLocalTranslation()));
                    if (autoRotation) {
                        mover.setLocalRotation(
                                curve.getOrientation(
                                1.0f - deltaTime,
                                orientationPrecision,
                                up));
                    }
                }
            } else {
                return;
            }
        }
    }

    private boolean isDisableAfterClamp() {   // ***** added
        return disableAfter;
    }

    public void setDisableAfterClamp(boolean disableAfter) {   // ***** added
        this.disableAfter = disableAfter;
    }

    @Override
    public void write(JMEExporter e) throws IOException {
        super.write(e);
        OutputCapsule capsule = e.getCapsule(this);
        capsule.write(mover, "mover", null);
        capsule.write(curve, "Curve", null);
        capsule.write(up, "up", null);
        capsule.write(orientationPrecision, "orientationPrecision", 0.1f);
        capsule.write(cycleForward, "cycleForward", true);
        capsule.write(autoRotation, "autoRotation", false);
    }

    @Override
    public void read(JMEImporter e) throws IOException {
        super.read(e);
        InputCapsule capsule = e.getCapsule(this);

        mover = (Spatial) capsule.readSavable("mover", null);
        curve = (Curve) capsule.readSavable("curve", null);
        up = (Vector3f) capsule.readSavable("up", null);
        orientationPrecision = capsule.readFloat("orientationPrecision", 0.1f);
        cycleForward = capsule.readBoolean("cycleForward", true);
        autoRotation = capsule.readBoolean("autoRotation", false);
    }
}
