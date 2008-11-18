package com.radakan.game.input;

import org.lwjgl.input.Keyboard;

import com.jme.input.lwjgl.LWJGLKeyInput;

public class LWJGLKeyInput2 extends LWJGLKeyInput {

	public LWJGLKeyInput2() {
		super();
		Keyboard.enableRepeatEvents(true);
	}

}
