#include <OgreException.h>

#include "engines/game.hpp"

using namespace std;

#ifdef RADAKAN_WINDOWS
	INT WINAPI WinMain (HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main (int argc, char * argv [])
#endif
{
	//	example arguments: C:/John/Radakan~C:/OgreSDK/Samples/Media
	//	example arguments: /home/john/radakan /usr/share/doc/ogre-1.4.0/Samples/Media

	try
	{
		Radakan :: Reference <Radakan :: Engines :: Game> game
			(new Radakan :: Engines :: Game ());

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
