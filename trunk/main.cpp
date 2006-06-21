#include "tslrpg.hpp"

using namespace std;

vector <Object *> objects;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain (HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main (int argc, char * argv [])
#endif
{
	// Create application object
	Tslrpg game;

	SET_TERM_HANDLER;

	try
	{
		game.go();
	}
	catch (Ogre::Exception & e)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox (NULL, e.getFullDescription().c_str (), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		cerr << "An exception has occured: " << e.getFullDescription () << endl;
#endif
	}

	return 0;
}
