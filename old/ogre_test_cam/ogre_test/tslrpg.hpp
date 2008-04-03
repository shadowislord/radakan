#ifndef __TSLRPG_H__
#define __TSLRPG_H__

using namespace Ogre;

class Tslrpg_Prototype
{

public:

	Tslrpg_Prototype () : ogre_root (0), ogre_scene_mgr (0), ogre_camera (0), frame_listener (0),
		player_char (0), ex_camera (0) { }
	~Tslrpg_Prototype ()
	{
		if (ex_camera) delete ex_camera;
		if (player_char) delete player_char;
		if (frame_listener) delete frame_listener;
		if (ogre_root) delete ogre_root;
	}

	void run (void);

protected:

	bool setup_ogre (void);
	bool setup_resources (void);
	bool setup_scene (void);

	Root* ogre_root;
	RenderWindow* ogre_window;
	SceneManager* ogre_scene_mgr;
	Camera* ogre_camera;

	Input* frame_listener;

	// 3rd person camera stuff
	Player_Character * player_char;
	Extended_Camera * ex_camera;

};

#endif

