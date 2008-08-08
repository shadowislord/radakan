/*
 * Copyright (c) 2007, MFKARPG
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name of the Gibbon Entertainment nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY 'Gibbon Entertainment' "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL 'Gibbon Entertainment' BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
package com.gibbon.jme.context;

import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.gibbon.radakan.config.ConfigFrame;
import com.jme.bounding.BoundingBox;
import com.jme.image.Texture;
import com.jme.image.Texture.EnvironmentalMapMode;
import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MinificationFilter;
import com.jme.image.Texture.WrapMode;
import com.jme.input.KeyInput;
import com.jme.light.PointLight;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Controller;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.shape.Box;
import com.jme.scene.shape.Torus;
import com.jme.scene.state.CullState;
import com.jme.scene.state.CullState.Face;
import com.jme.scene.state.LightState;
import com.jme.scene.state.TextureState;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;
import com.jme.system.GameSettings;
import com.jme.system.PreferencesGameSettings;
import com.jme.util.TextureManager;
import com.jme.util.export.binary.BinaryImporter;
import com.jmex.model.converters.MaxToJme;
import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.prefs.Preferences;

/**
 * Scene setup from GLSL demos
 * @author Momoko_Fan
 */
public class TestJmeContext {

	private static Node createObjects() {
		Node objects = new Node("objects");

		Torus torus = new Torus("Torus", 50, 50, 10, 20);
		torus.setLocalTranslation(new Vector3f(50, -5, 20));
		TextureState ts = JmeContext.get().getRenderer().createTextureState();
		Texture t0 = null;
		Texture t1 = null;
		try {
			t0 = TextureManager.loadTexture(new File("/home/tomas/NetBeansProjects/Radakan/jmetest/data/images/Monkey.jpg").toURL(), MinificationFilter.Trilinear, MagnificationFilter.Bilinear);
		} catch (MalformedURLException ex) {
			Logger.getLogger(TestJmeContext.class.getName()).log(Level.SEVERE, null, ex);
		}
		
		try {
			t1 = TextureManager.loadTexture(new File("/home/tomas/NetBeansProjects/Radakan/jmetest/data/texture/north.jpg").toURL(), MinificationFilter.Trilinear, MagnificationFilter.Bilinear);
		} catch (MalformedURLException ex) {
			Logger.getLogger(TestJmeContext.class.getName()).log(Level.SEVERE, null, ex);
		}
		t1.setEnvironmentalMapMode(EnvironmentalMapMode.SphereMap);
		ts.setTexture(t0, 0);
		ts.setTexture(t1, 1);
		ts.setEnabled(true);
		torus.setRenderState(ts);
		objects.attachChild(torus);

		ts = JmeContext.get().getRenderer().createTextureState();
		try {
			t0 = TextureManager.loadTexture(new File("/home/tomas/NetBeansProjects/Radakan/jmetest/data/texture/wall.jpg").toURL(), MinificationFilter.Trilinear, MagnificationFilter.Bilinear);
		} catch (MalformedURLException ex) {
			Logger.getLogger(TestJmeContext.class.getName()).log(Level.SEVERE, null, ex);
		}
		t0.setWrap(WrapMode.Repeat);
		ts.setTexture(t0);

		Box box = new Box("box1", new Vector3f(-10, -10, -10), new Vector3f(10, 10, 10));
		box.setLocalTranslation(new Vector3f(0, -7, 0));
		box.setRenderState(ts);
		objects.attachChild(box);

		box = new Box("box2", new Vector3f(-5, -5, -5), new Vector3f(5, 5, 5));
		box.setLocalTranslation(new Vector3f(15, 10, 0));
		box.setRenderState(ts);
		objects.attachChild(box);

		box = new Box("box3", new Vector3f(-5, -5, -5), new Vector3f(5, 5, 5));
		box.setLocalTranslation(new Vector3f(0, -10, 15));
		box.setRenderState(ts);
		objects.attachChild(box);

		box = new Box("box4", new Vector3f(-5, -5, -5), new Vector3f(5, 5, 5));
		box.setLocalTranslation(new Vector3f(20, 0, 0));
		box.setRenderState(ts);
		objects.attachChild(box);

		box = new Box("box5", new Vector3f(-50, -2, -50), new Vector3f(50, 2, 50));
		box.setLocalTranslation(new Vector3f(0, -15, 0));
		box.setRenderState(ts);
		box.setModelBound(new BoundingBox());
		box.updateModelBound();
		objects.attachChild(box);

		ts = JmeContext.get().getRenderer().createTextureState();
		try {
			t0 = TextureManager.loadTexture(new File("/home/tomas/NetBeansProjects/Radakan/jmetest/data/texture/cloud_land.jpg").toURL(), MinificationFilter.Trilinear, MagnificationFilter.Bilinear);
		} catch (MalformedURLException ex) {
			Logger.getLogger(TestJmeContext.class.getName()).log(Level.SEVERE, null, ex);
		}
		t0.setWrap(WrapMode.Repeat);
		ts.setTexture(t0);

		box = new Box("floor", new Vector3f(-1000, -10, -1000), new Vector3f(1000, 10, 1000));
		box.setLocalTranslation(new Vector3f(0, -100, 0));
		box.setRenderState(ts);
		box.setModelBound(new BoundingBox());
		box.updateModelBound();
		objects.attachChild(box);

		return objects;
	}

	public static Spatial initRoot() {
		Node rootNode = new Node("rootNode");

		CullState cull = JmeContext.get().getRenderer().createCullState();
		cull.setCullFace(Face.Back);
		rootNode.setRenderState(cull);

		ZBufferState zbuf = JmeContext.get().getRenderer().createZBufferState();
		zbuf.setFunction(TestFunction.LessThanOrEqualTo);
		zbuf.setWritable(true);
		rootNode.setRenderState(zbuf);

		LightState lightState = JmeContext.get().getRenderer().createLightState();
		rootNode.setRenderState(lightState);

		//Setup lights
		PointLight light = new PointLight();
		light.setDiffuse(new ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f));
		light.setAmbient(new ColorRGBA(0.5f, 0.5f, 0.5f, 1.0f));
		light.setLocation(new Vector3f(0, 30, 0));
		light.setEnabled(true);
		lightState.attach(light);

		//Add dummy objects to rootNode
		rootNode.attachChild(createObjects());

		try {
			MaxToJme C1 = new MaxToJme();
			ByteArrayOutputStream BO = new ByteArrayOutputStream();
//            URL maxFile = TestJmeContext.class.getClassLoader().getResource("/home/tomas/NetBeansProjects/Radakan/jmetest/data/model/Character.3DS");
			URL maxFile = new File("/home/tomas/NetBeansProjects/Radakan/jmetest/data/model/Character.3DS").toURL();

			C1.convert(new BufferedInputStream(maxFile.openStream()), BO);
			Node r = (Node) BinaryImporter.getInstance().load(new ByteArrayInputStream(BO.toByteArray()));
			r.getController(0).setRepeatType(Controller.RT_WRAP);
			r.setLocalScale(.1f);
			if (r.getChild(0).getControllers().size() != 0) {
				r.getChild(0).getController(0).setSpeed(20);
			}
			Quaternion temp = new Quaternion();
			temp.fromAngleAxis(FastMath.PI / 2.0f, new Vector3f(-1, 0, 0));
			r.setLocalRotation(temp);
			r.setLocalTranslation(new Vector3f(0, 3, 0));
			rootNode.attachChild(r);
		} catch (IOException e) {
			e.printStackTrace();
		}

		rootNode.updateGeometricState(0, true);
		rootNode.updateRenderState();

		return rootNode;
	}

	public static void main(String[] args) throws InterruptedException, InstantiationException {
		GameSettings settings = new PreferencesGameSettings(Preferences.userRoot().node("test-jmecontext"));

		settings.set("title", "Test JmeContext -  Delete/PgDn for eye distance, Home/End for focal distance");

		ConfigFrame g = new ConfigFrame(settings);
		g.setVisible(true);
		g.waitFor();

		JmeContext cx = JmeContext.create(LWJGLContext.class, JmeContext.CONTEXT_WINDOW);
		cx.setSettings(settings);
		cx.start();
		cx.waitFor();

		cx.getPassManager().loadDefaultPasses();

		final StereoRenderPass rp = new StereoRenderPass();
		rp.setMode(StereoRenderPass.StereoMode.ANAGLYPH);
		rp.setProjection(StereoRenderPass.ProjectionMode.ASYMMETRIC_FRUSTUM);
		rp.setEyeDistance(3.0f);

		rp.add(initRoot());

		cx.getPassManager().add(rp);

		UpdatePass up = new UpdatePass() {

			@Override
			public void runPass(JmeContext cx) {
				super.runPass(cx);
				float tpf = cx.getPassManager().getTPF();

				if (KeyInput.get().isKeyDown(KeyInput.KEY_DELETE)) {
					rp.setEyeDistance(rp.getEyeDistance() + 0.1f);
				} else if (KeyInput.get().isKeyDown(KeyInput.KEY_PGDN)) {
					rp.setEyeDistance(rp.getEyeDistance() - 0.1f);
				}
				if (KeyInput.get().isKeyDown(KeyInput.KEY_HOME)) {
					rp.setFocusDistance(rp.getFocusDistance() + 1);
				} else if (KeyInput.get().isKeyDown(KeyInput.KEY_END)) {
					rp.setFocusDistance(rp.getFocusDistance() - 1);
				}
			}
		};

		cx.getPassManager().add(up);
	}
}
