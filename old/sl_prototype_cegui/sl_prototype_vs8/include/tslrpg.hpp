#ifndef __TSLRPG_H__
#define __TSLRPG_H__

#include <OgreNoMemoryMacros.h>
#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
#include "OgreCEGUIRenderer.h"
#include "OgreCEGUIResourceProvider.h"
#include <OgreMemoryMacros.h> 

#include <Ogre.h>

using namespace Ogre;

class Tslrpg_Prototype
{

public:

	Tslrpg_Prototype () : ogre_root (0), ogre_scene_mgr (0), ogre_camera (0), ois_input (0),
		cegui_renderer (0), cegui_system (0), cegui_window (0) { }
	~Tslrpg_Prototype ()
	{
		if (cegui_window) CEGUI::WindowManager::getSingleton ().destroyWindow (cegui_window);
		if (cegui_system) delete cegui_system;
		if (cegui_renderer) delete cegui_renderer;

		if (ois_input) delete ois_input;
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

	Input* ois_input;

	CEGUI::OgreCEGUIRenderer* cegui_renderer;
	CEGUI::System* cegui_system;
	CEGUI::Window* cegui_window;

};

#endif

