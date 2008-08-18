/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.test.context;

import com.gibbon.jme.util.FengJMEInputHandler;
import com.gibbon.jme.util.FengJMEListener;
import org.fenggui.composite.Window;
 
import com.jme.app.BaseGame;
import com.jme.input.KeyBindingManager;
import com.jme.input.KeyInput;
import com.jme.input.MouseInput;
import com.jme.light.PointLight;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Node;
import com.jme.scene.shape.Box;
import com.jme.scene.state.LightState;
import com.jme.scene.state.ZBufferState;
import com.jme.system.DisplaySystem;
import com.jme.system.JmeException;
import com.jme.util.Timer;
import com.jme.util.lwjgl.LWJGLTimer;
 
import com.jme.scene.state.ZBufferState.TestFunction;
import org.fenggui.Button;
import org.fenggui.CheckBox;
import org.fenggui.Container;
import org.fenggui.FengGUI;
import org.fenggui.Label;
import org.fenggui.RadioButton;
import org.fenggui.ToggableGroup;
import org.fenggui.event.ButtonPressedEvent;
import org.fenggui.event.IButtonPressedListener;
import org.fenggui.layout.RowLayout;
import org.fenggui.util.Point;
import org.fenggui.util.Spacing;
 
import org.lwjgl.opengl.GL13;
/**
 * FengJME - A test class for integrating FengGUI and jME.
 *
 * @author Josh (updated by neebie)
 *
 */
public class FengJME extends BaseGame
{
	Camera cam; // Camera for jME
	Node rootNode; // The root node for the jME scene
	PointLight light; // Changeable light
	FengJMEInputHandler input;
	Timer timer;
 
	Box box; // A box
 
	org.fenggui.Display disp; // FengGUI's display
 
 
	/* (non-Javadoc)
	 * @see com.jme.app.BaseGame#cleanup()
	 */
	@Override
	protected void cleanup()
	{
		// Clean up the mouse
		MouseInput.get().removeListeners();
		MouseInput.destroyIfInitalized();
		// Clean up the keyboard
		KeyInput.destroyIfInitalized();
	}
 
 
	/* (non-Javadoc)
	 * @see com.jme.app.BaseGame#initGame()
	 */
	@Override
	protected void initGame()
	{
 
		// Create our root node
		rootNode = new Node("rootNode");
		// Going to enable z-buffering
		ZBufferState buf = display.getRenderer().createZBufferState();
		buf.setEnabled(true);
		buf.setFunction(TestFunction.LessThan);
		// ... and set the z-buffer on our root node
		rootNode.setRenderState(buf);
 
		// Create a white light and enable it
		light = new PointLight();
		light.setDiffuse(new ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f));
		light.setAmbient(new ColorRGBA(0.5f, 0.5f, 0.5f, 1.0f));
		light.setLocation(new Vector3f(100, 100, 100));
		light.setEnabled(true);
 
		/** Attach the light to a lightState and the lightState to rootNode. */
 
		LightState lightState = display.getRenderer().createLightState();
		lightState.setEnabled(true);
		lightState.attach(light);
		rootNode.setRenderState(lightState);
 
		// Create our box
		box = new Box("The Box", new Vector3f(-1, -1, -1), new Vector3f(1, 1, 1));
		box.updateRenderState();
		// Rotate the box 25 degrees along the x and y axes.
		Quaternion rot = new Quaternion();
		rot.fromAngles(FastMath.DEG_TO_RAD * 25, FastMath.DEG_TO_RAD * 25, 0.0f);
		box.setLocalRotation(rot);
		// Attach the box to the root node
		rootNode.attachChild(box);
 
		// Update our root node
		rootNode.updateGeometricState(0.0f, true);
		rootNode.updateRenderState();
 
 
		// Create the GUI
		initGUI();
	}
 
 
	/**
	 * Create our GUI.  FengGUI init code goes in here
	 *
	 */
	protected void initGUI()
	{
		// Grab a display using an LWJGL binding
		//	   (obviously, since jME uses LWJGL)
		disp = new org.fenggui.Display(new org.fenggui.binding.render.lwjgl.LWJGLBinding());
 
		input = new FengJMEInputHandler(disp);
                
                //new FengJMEListener(disp);
 
		final Window dialog = FengGUI.createWindow(disp, true, false, false, true);
		final Label label = FengGUI.createLabel(dialog.getContentContainer(),"---");
		dialog.setTitle("you ordered:");
		dialog.setPosition(new Point(250,250));
		dialog.pack();
 
		Window window = FengGUI.createWindow(disp, true, false, false, true);
		window.setTitle("hot drinks");
		window.setPosition(new Point(50,200));
		window.getContentContainer().setLayoutManager(new RowLayout(false));
		window.getContentContainer().getAppearance().setPadding(new Spacing(10, 10));
 
		final ToggableGroup<String> toggableGroup = new ToggableGroup<String>();
		Container container = new Container(new RowLayout(true));
		RadioButton<String> radioButtonCoffee = FengGUI.createRadioButton(container, "coffee", toggableGroup);
		RadioButton<String> radioButtonTea = FengGUI.createRadioButton(container, "tea", toggableGroup);
		radioButtonCoffee.setValue("coffee");
		radioButtonTea.setValue("tea");
		radioButtonTea.setSelected(true);
		window.getContentContainer().addWidget(container);
		final CheckBox<String> checkBoxMilk = FengGUI.createCheckBox(window.getContentContainer(), "milk");
		final CheckBox<String> checkBoxSugar = FengGUI.createCheckBox(window.getContentContainer(), "sugar");
 
		Button button = FengGUI.createButton(window.getContentContainer(), "submit your order");
		button.addButtonPressedListener(new IButtonPressedListener() {
			public void buttonPressed(ButtonPressedEvent arg0) {
 
				String temp = toggableGroup.getSelectedValue();
				if (checkBoxMilk.isSelected()) {
					if (checkBoxSugar.isSelected())
						temp += " with milk and sugar.";
					else
						temp += " with milk.";
				}
				else
					if (checkBoxSugar.isSelected())
						temp += " with sugar.";
 
				label.setText(temp);
				dialog.pack();
			}
		});
 
		window.pack();
 
		// Update the display with the newly added components
		disp.layout();
	}
 
 
	/* (non-Javadoc)
	 * @see com.jme.app.BaseGame#initSystem()
	 */
	@Override
	protected void initSystem()
	{
            
		try
		{
			// Initialize our jME display system
			display = DisplaySystem.getDisplaySystem(settings.getRenderer());
			display.createWindow(settings.getWidth(), settings.getHeight(), settings.getDepth(), settings.getFrequency(), settings.isFullscreen());
 
			// Get a camera based on the window settings
			cam = display.getRenderer().createCamera(display.getWidth(), display.getHeight());
		}
		catch (JmeException ex)
		{
			ex.printStackTrace();
			System.exit(1);
		}
 
		// We want a cursor to interact with FengGUI
		MouseInput.get().setCursorVisible(true);
 
		// Bind the Escape key to kill our test app
		KeyBindingManager.getKeyBindingManager().set("quit", KeyInput.KEY_ESCAPE);
 
		// Set a black background.
		display.getRenderer().setBackgroundColor(ColorRGBA.black);
		// Set up how our camera sees.
		cam.setFrustumPerspective(45.0f, (float) display.getWidth() / (float) display.getHeight(), 1, 1000);
		Vector3f loc = new Vector3f(0.0f, 0.0f, 15.0f);
		Vector3f left = new Vector3f(-1.0f, 0.0f, 0.0f);
		Vector3f up = new Vector3f(0.0f, 1.0f, 0.0f);
		Vector3f dir = new Vector3f(0.0f, 0f, -1.0f);
		//  Move our camera to a correct place and orientation.
		cam.setFrame(loc, left, up, dir);
		//  Signal that we've changed our camera's location/frustum.
		cam.update();
		//  Assign the camera to this renderer.
		display.getRenderer().setCamera(cam);
 
		// Create our timer
		timer = new LWJGLTimer();
	}
 
 
	/* (non-Javadoc)
	 * @see com.jme.app.BaseGame#reinit()
	 */
	@Override
	protected void reinit() {}
 
 
	/* (non-Javadoc)
	 * @see com.jme.app.BaseGame#render(float)
	 */
	@Override
	protected void render(float interpolation)
	{
		// Clear previous
		display.getRenderer().clearBuffers();
 
		// Draw jME stuff
		//display.getRenderer().draw(rootNode);
 
		// Set back to first texture unit so GUI displays properly
		GL13.glActiveTexture(GL13.GL_TEXTURE0);
 
		// Draw GUI
		disp.display();
	}
 
 
	/* (non-Javadoc)
	 * @see com.jme.app.BaseGame#update(float)
	 */
	@Override
	protected void update(float interpolation)
	{
		timer.update();
		float tpf = timer.getTimePerFrame();
		input.update(tpf);
		if (!input.wasKeyHandled())
		{
			// Check to see if Escape was pressed
			if (KeyBindingManager.getKeyBindingManager().isValidCommand("quit")) finish();
		}
	}
 
 
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		FengJME app = new FengJME();
		app.setConfigShowMode(ConfigShowMode.AlwaysShow);
		app.start();
	}
}