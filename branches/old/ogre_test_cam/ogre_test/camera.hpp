#ifndef CAMERA_HPP
#define CAMERA_HPP

using namespace Ogre;
using namespace std;

// Our extended camera class
class Extended_Camera
{

public:

	Extended_Camera (String cam_name, SceneManager * ogre_scene_mgr, Camera * ogre_camera)
	{
		// Basic member references setup
		name = cam_name;
		scene_mgr = ogre_scene_mgr;
		camera = ogre_camera;

		// Create the camera's node structure
		camera_node = scene_mgr -> getRootSceneNode () -> createChildSceneNode (name);
		target_node = scene_mgr -> getRootSceneNode () -> createChildSceneNode (name + "_target");

		camera_node -> setAutoTracking (true, target_node); // The camera will always look at the camera target
		camera_node -> setFixedYawAxis (true); // Needed because of auto tracking

		// attach the Ogre camera to the camera node
		camera_node -> attachObject (camera);

		// Default tightness
		tightness = 0.01f;
	}

	~Extended_Camera ()
	{
		camera_node -> detachAllObjects ();
		scene_mgr -> destroySceneNode (name + "_target");
		scene_mgr -> destroySceneNode (name);
	}

	void set_tightness (Real tight) { tightness = tight; }
	Real get_tightness () { return tightness; }

	Vector3 get_camera_position () { return camera_node -> getPosition (); }

	void instant_update (Vector3 cam_pos, Vector3 target_pos)
	{
		camera_node -> setPosition (cam_pos);
		target_node -> setPosition (target_pos);
	}

	void update (Real elapsed_time, Vector3 cam_pos, Vector3 target_pos)
	{
		// Handle movement
		Vector3 displacement;

		displacement = (cam_pos - camera_node -> getPosition ()) * tightness;
		camera_node -> translate (displacement);

		displacement = (target_pos - target_node -> getPosition ()) * tightness;
		target_node -> translate (displacement);
	}

protected:

	String name;

	Camera * camera; // Ogre camera

	SceneNode * camera_node; // The camera itself
	SceneNode * target_node; // The camera target

	SceneManager * scene_mgr;

	Real tightness; // Determines the movement of the camera - 1 means tight movement, while 0 means no movement

};

#endif