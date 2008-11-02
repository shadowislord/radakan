package com.radakan.game.input;

/**
 * Provides a means for the game to recieve input events in a controller manner.
 * E.g the notifications will only be made if the game is currently in the "InGame" state,
 * and no windows are obsucring the screen.
 * 
 * @author Kirill Vainer
 */
public interface IGameInputHandler {

    public void onButtonPress(String buttonName, boolean pressed);
    public void onButtonHold(String buttonName);
    
    public void onAxisPress(String axisName, boolean pressed);
    public void onAxisHold(String axisName, float value);
    
}
