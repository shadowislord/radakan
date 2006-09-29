/*
	I have pretty much created this file by directly looking at the Ogre 3D source code.
	It seems to be working so use this if you can.
*/
#include <Ogre.h>

#include "prototype_application.hpp"

// ----------------------------------------------------------------------------
// Main function, just boots the application object
// ----------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN
	#include "windows.h"
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char **argv)
#endif
{
    PrototypeApplication *app = new PrototypeApplication();

    app->Run();

	//		MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);

    return 0;
}

