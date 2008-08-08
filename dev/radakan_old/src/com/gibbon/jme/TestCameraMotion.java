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

package com.gibbon.jme;

import com.jme.app.SimpleGame;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.CameraNode;
import com.jme.scene.Controller;
import com.jme.scene.shape.Box;
import com.jme.util.geom.BufferUtils;

/**
 * Tests the CurveOnceController
 * 
 * @author Momoko_Fan
 */
public class TestCameraMotion extends SimpleGame {
    
    private ColorRGBA[] colors;
    private CatmullRomCurve curve;
    private CameraNode camNode;

    public static void main(String[] args){
        TestCameraMotion test = new TestCameraMotion();
        test.setConfigShowMode(ConfigShowMode.AlwaysShow);
        test.start();
    }
    
    @Override
    protected void simpleInitGame() {
        camNode = new CameraNode("Cam", display.getRenderer().getCamera());
        
        Vector3f[] points = new Vector3f[2];
        points[0] = new Vector3f(-4, 0, 0);
        points[1] = new Vector3f(4, 0, 0);

        curve = new CatmullRomCurve("Curve", points);
        colors = new ColorRGBA[2];
        colors[0] = new ColorRGBA(0, 1, 0, 1);
        colors[1] = new ColorRGBA(0, 0, 1, 1);
        curve.setColorBuffer(BufferUtils.createFloatBuffer(colors));

        CurveOnceController cc = new CurveOnceController(curve, camNode);
        cc.setActive(false);
        camNode.addController(cc);
        cc.setRepeatType(Controller.RT_CYCLE);
        cc.setUpVector(Vector3f.UNIT_Y);
        cc.setSpeed(0.10f);
        //cc.setAutoRotation(true);
        //cc.setDisableAfterClamp(true);
        
        Box b = new Box("Box", new Vector3f(5, 0, 5), 2, 2, 2);
        LookAtController lac = new LookAtController(camNode, b);
        camNode.addController(lac);
        
        rootNode.attachChild(b);
        //rootNode.attachChild(camNode);
        //rootNode.attachChild(curve);
        rootNode.updateRenderState();

//        //...on some action
//        Vector3f[] points = new Vector3f[2];
//        points[0] = cam.getLocation();
//        points[1] = endPosition;
//
//        curve.reconstruct(
//                BufferUtils.createFloatBuffer(points),
//                null,
//                BufferUtils.createFloatBuffer(colors),
//                null);
//        cc.setActive(true);
    }
}
