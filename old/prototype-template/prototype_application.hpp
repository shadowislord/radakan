#ifndef __PROTOTYPE_APPLICATION__
#define __PROTOTYPE_APPLICATION__

#include "input.hpp"

#include <Ogre.h>

#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
#include <CEGUI/elements/CEGUICombobox.h>
#include <CEGUI/elements/CEGUIListbox.h>
#include <CEGUI/elements/CEGUIListboxTextItem.h>
#include <CEGUI/elements/CEGUIPushButton.h>
#include <CEGUI/elements/CEGUIScrollbar.h>
#include <CEGUI/elements/CEGUIStaticImage.h>
#include "OgreCEGUIRenderer.h"
#include "OgreCEGUIResourceProvider.h"

using namespace Ogre;

class PrototypeApplication
{
public:
	// Initialise the application
	PrototypeApplication();

	void Load_Resources();

	// Create the scene
	void Create_Scene();

	// Start the game loop
	bool Run();

protected:

private:
	Root			*ogre_root;
	RenderWindow	*ogre_window;
	RenderSystem	*ogre_renderer;
	PlatformManager *ogre_platform_manager;
	SceneManager 	*ogre_scene_manager;

	// Input handling
	Input *input;

	// Game specific objects
	Camera *ogre_camera;

	// CEGUI stuff
	CEGUI::OgreCEGUIRenderer *cegui_renderer;
    CEGUI::System* cegui_system;
    CEGUI::Window* cegui_window;

};


#endif
