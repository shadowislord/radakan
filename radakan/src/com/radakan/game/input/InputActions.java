package com.radakan.game.input;

public final class InputActions {
	private InputActions() {}
	
	// Camera Actions
	public static String CAMERA_ROTATE_LEFT = "CAMERA_ROTATE_LEFT";
	public static String CAMERA_ROTATE_RIGHT = "CAMERA_ROTATE_RIGHT";
	public static String CAMERA_ROTATE_UP = "CAMERA_ROTATE_UP";
	public static String CAMERA_ROTATE_DOWN = "CAMERA_ROTATE_DOWN";
	public static String CAMERA_ZOOM_OUT = "CAMERA_ZOOM_OUT";
	public static String CAMERA_ZOOM_IN = "CAMERA_ZOOM_IN";
	
	// Movement Actions
	public static String PLAYER_MOVE_FWD = "PLAYER_MOVE_FWD";
	public static String PLAYER_MOVE_BACK = "PLAYER_MOVE_BACK";
	public static String PLAYER_TURN_LEFT = "PLAYER_TURN_LEFT";
	public static String PLAYER_TURN_RIGHT = "PLAYER_TURN_RIGHT";
}
