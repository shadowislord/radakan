#ifndef __PROTOTYPE_APPLICATION__
#define __PROTOTYPE_APPLICATION__

#include <Ogre.h>

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

	// Game specific objects
	Camera *ogre_camera;
};


#endif
