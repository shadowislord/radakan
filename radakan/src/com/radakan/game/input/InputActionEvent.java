package com.radakan.game.input;

public class InputActionEvent {
	public String name;
	public int x, y, velocity;
	
	public InputActionEvent(String name, int x, int y, int velocity) {
		this.name = name;
		this.x = x;
		this.y= y;
		this.velocity = velocity;
	}
}
