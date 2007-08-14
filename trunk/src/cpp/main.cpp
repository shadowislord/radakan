#include "game.hpp"
#include <OgreException.h>

using namespace std;

#ifdef RADAKAN_WINDOWS
	INT WINAPI WinMain (HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main (int argc, char * argv [])
#endif
{
	string radakan_path;
	string ogre_media_path;
	#ifdef RADAKAN_WINDOWS
		//	example arguments: C:/John/Radakan~C:/OgreSDK/Samples/Media
		radakan_path = strCmdLine;
		if (radakan_path . empty ())
		{
			TCHAR buffer [MAX_PATH];
			GetCurrentDirectory (MAX_PATH, buffer);
			radakan_path = buffer;
		}

		unsigned int spacePos = radakan_path . find ('~');
		assert (spacePos != string :: npos);	//	did you pass the arguments correctly?
		ogre_media_path = radakan_path . substr (spacePos + 1, radakan_path . length ());
		radakan_path = radakan_path . substr (0, spacePos);
	#else
		//	example arguments: /home/john/radakan /usr/share/doc/ogre-1.4.0/Samples/Media
		assert (2 <= argc);	//	did you pass the arguments correctly?
		radakan_path = argv [argc - 2];
		ogre_media_path = argv [argc - 1];
	#endif

	try
	{
		Radakan :: Engines :: Game game (radakan_path, ogre_media_path);

		game . run ();
		
		//	The game is automatically shut down here.
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
