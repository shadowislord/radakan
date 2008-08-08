#ifndef __TSLRPG_H__
#define __TSLRPG_H__

#include "Ogre.h"

using namespace Ogre;

class Tslrpg_Prototype
{

public:

	Tslrpg_Prototype () : root (0), scene_mgr (0), camera (0), input (0) { }
	~Tslrpg_Prototype ()
	{
		if (input) delete input;
		if (root) delete root;
	}

	void run (void);

protected:

	bool setup_ogre (void);
	bool setup_resources (void);
	bool setup_scene (void);


	Root* root;
	RenderWindow* window;
	SceneManager* scene_mgr;
	Camera* camera;

	Input* input;

};

#endif

