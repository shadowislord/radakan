#include "game.hpp"

using namespace std;

#ifdef TSL_WIN
	INT WINAPI WinMain (HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main (int argc, char * argv [])
#endif
{
	string tsl_path;
	string ogre_media_path;
	#ifdef TSL_WIN
		//	example arguments: C:/John/TSL~C:/OgreSDK/Samples/Media
		tsl_path = strCmdLine;
		if (tsl_path . empty ())
		{
			TCHAR buffer [MAX_PATH];
			GetCurrentDirectory (MAX_PATH, buffer);
			tsl_path = buffer;
		}

		unsigned int spacePos = tsl_path . find ('~');
		assert (spacePos != string :: npos);	//	did you pass the arguments correctly?
		ogre_media_path = tsl_path . substr (spacePos + 1, tsl_path . length ());
		tsl_path = tsl_path . substr (0, spacePos);
	#else
		//	example arguments: /home/john/tsl /usr/share/doc/ogre-1.4.0/Samples/Media
		assert (2 <= argc);	//	did you pass the arguments correctly?
		tsl_path = argv [argc - 2];
		ogre_media_path = argv [argc - 1];
	#endif

	try
	{
		TSL :: Engines :: Game game (tsl_path, ogre_media_path);

		game . run ();
		
		//	The game is automatically shut down here.
	}
	catch (Ogre :: Exception & exception)
	{
		#ifdef TSL_WIN
			MessageBox
				(NULL, exception . getFullDescription () . c_str (),
				"An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
			cerr << "Exception: " << exception . getFullDescription () << endl;
		#endif
	}

	return 0;
}
