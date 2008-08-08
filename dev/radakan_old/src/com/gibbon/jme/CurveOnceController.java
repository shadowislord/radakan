/*
 * Copyright (c) 2003-2006 jMonkeyEngine
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of 'jMonkeyEngine' nor the names of its contributors 
 *   may be used to endorse or promote products derived from this software 
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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

/**
 * This is a curve controller for cameras and cinematic sequences..
 * 
 * @author Frederik Bülthoff (?)
 */
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

    /**
     * Create a new CurveOnce controller that moves the spatial <code>mover</code>
     * with the path specified by <code>curve</code>
     * 
     * @param curve The curve to control the spatial
     * @param mover The spatial to control
     */
    public CurveOnceController(Curve curve, Spatial mover) {
        this.curve = curve;
        this.mover = mover;
        setUpVector(new Vector3f(0, 1, 0));
        setMinTime(0);
        setMaxTime(Float.MAX_VALUE);
        setRepeatType(Controller.RT_CLAMP);
        setSpeed(1.0f);
    }

    /**
     * A CurveOnceController that is only effected by certain times
     * Can be used in a setup where the camera has to move on the curve only
     * at certain times.
     * 
     * @param curve
     * @param mover
     * @param minTime
     * @param maxTime
     */
    public CurveOnceController(Curve curve, Spatial mover, float minTime, float maxTime){
        this.curve = curve;
        this.mover = mover;
        setMinTime(minTime);
        setMaxTime(maxTime);
        setRepeatType(Controller.RT_CLAMP);
    }

    /**
     * Sets the UP vector<br/>
     * This is usually 0, 1, 0 in jME and Radakan
     * 
     * @param up
     */
    public void setUpVector(Vector3f up) {
        this.up = up;
    }

    /**
     * I don't know what this does..
     * 
     * @param value
     */
    public void setOrientationPrecision(float value) {
        orientationPrecision = value;
    }

    /**
     * If true, rotate the spatial in addition to moving it
     * 
     * @param value True to rotate the spatial
     */
    public void setAutoRotation(boolean value) {
        autoRotation = value;
    }

    /**
     * @return Whether to rotate the spatial to face the curve or not
     */
    public boolean isAutoRotating() {
        return autoRotation;
    }

    /**
     * Updates this controller, should only be used internally by jME.
     * 
     * @param time Time per frame (1 / fps)
     */
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

    /**
     * @return If true, disable this controller when the end time is reached
     */
    private boolean isDisableAfterClamp() {   // ***** added
        return disableAfter;
    }

    /**
     * Set whether to disable this controller when the end time is reached
     * 
     * @param disableAfter
     * @see #setTimes
     */
    public void setDisableAfterClamp(boolean disableAfter) {   // ***** added
        this.disableAfter = disableAfter;
    }

    /**
     * Serialize this Controller
     * 
     * @param e
     * @throws java.io.IOException
     */
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

    /**
     * De-serialize this controller
     * 
     * @param e
     * @throws java.io.IOException
     */
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
