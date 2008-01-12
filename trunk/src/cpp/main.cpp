#include <OgreException.h>

#include "engines/game.hpp"

using namespace std;

#ifdef RADAKAN_WINDOWS
	INT WINAPI WinMain (HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main (int argc, char * argv [])
#endif
{
	string path_to_config;

	// Here we get the optional path to where the Radakan configuration file is kept.
	#ifdef RADAKAN_WINDOWS
		path_to_config = strCmdLine;
		if (path_to_config . empty ())
		{
			TCHAR buffer [MAX_PATH];
			GetCurrentDirectory (MAX_PATH, buffer);
			path_to_config = buffer;
		}
	#else
		// Check to see if a parameter was passed
		if(argc>0)
		{
			// Assuming 0 is he program name, we just want the first parameter
			path_to_config = argv[1];
		}
		else
		{
			path_to_config = "./";
		}
	#endif


	try
	{
		Radakan :: Reference <Radakan :: Engines :: Game> game
			(new Radakan :: Engines :: Game (path_to_config));

		game -> run ();
		
		//	The game is automatically shut down here.
		game . destruct ();
	}
	catch (Ogre :: Exception & exception)
	{
		#ifdef RADAKAN_WINDOWS
			MessageBox
				(NULL, exception . getFullDescription () . c_str (),
				"An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
			cerr << "Exception: " << exception . getFullDescription () << endl;
		#endif
	}

	return 0;
}
