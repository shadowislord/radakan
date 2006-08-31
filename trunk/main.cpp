#include "tslrpg.hpp"

using namespace std;
using namespace Ogre;

#ifdef SL_DEBUG
	set <Object *> objects;
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain (HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main (int argc, char * argv [])
#endif
{
	try
	{
		// Create application object
		Tslrpg * game;
		
		cout << "Setting up Scattered Lands..." << endl;
		game = new Tslrpg ();
		cout << "Scattered Lands is set up." << endl;
		
		cout << "Running Scattered Lands..." << endl;
		game->run ();
		cout << "Scattered Lands is stopped." << endl;
		
		cout << "Shutting down Scattered Lands..." << endl;
		delete game;
		assert (objects.empty ());
		cout << "Scattered Lands is shut down." << endl;
	}
	catch (Exception & e)
	{
		#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox (NULL, e.getFullDescription ().c_str (), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
			cerr << "An exception has occured: " << e.getFullDescription () << endl;
		#endif
	}

	return 0;
}
