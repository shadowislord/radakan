package com.radakan.game.input;

public interface IGameRawInputListener extends IGameInputListener {
	public boolean onButtonPress(int button, boolean pressed, int x, int y);
    
    public boolean onMove( int xDelta, int yDelta, int newX, int newY );
    
    public boolean onWheel( int wheelDelta, int x, int y );
    
    public boolean onKey(char character, int keyCode, boolean pressed);
}
