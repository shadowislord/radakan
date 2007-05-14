#include "game.hpp"

using namespace std;
using namespace TSL;

#ifdef TSL_WIN
	INT WINAPI WinMain (HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main (int argc, char * argv [])
#endif
{
	cout << "Preparing The Scattered Lands..." << endl;

	//	check for conflicting debug and trace flags
	#ifdef TSL_TRACE
		#ifndef TSL_DEBUG
			cerr << "Conflicting TSL_DEBUG and TSL_TRACE flags detected." << endl;
			abort ();
		#endif
	#endif

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

	#ifdef TSL_DEBUG
		cout << "debug mode: enabled." << endl;
		#ifdef TSL_TRACE
			cout << "trace mode: enabled." << endl;
		#else
			cout << "trace mode: disabled." << endl;
		#endif

		//	'cout' is redirected to a log file.
		//	Don't use 'cout' or 'cerr'.
		cout . rdbuf ((new ofstream ((tsl_path + "/log/log.txt") . c_str ())) -> rdbuf ());

		cout << "tsl_path: " << tsl_path << endl;
		cout << "ogre_media_path: " << ogre_media_path << endl;
	#else
		cout << "debug mode: disabled." << endl;

		//	From here on, all cout messages are ignored.
		cout . rdbuf ((new ostream (new stringbuf (ios_base :: out))) -> rdbuf ());
	#endif

	try
	{
		cout << "Setting up The Scattered Lands..." << endl;
		Game game (tsl_path, ogre_media_path);
		cout << "The Scattered Lands is set up." << endl;

		cout << "Running The Scattered Lands..." << endl;
		game . run ();
		cout << "The Scattered Lands is stopped." << endl;

		cout << "Shutting down The Scattered Lands..." << endl;
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

	#ifdef TSL_DEBUG
/*		for (Object * i = Object :: objects . get_child (); i != NULL;
			i = Object :: objects . get_another_child ())
		{
			cout << "Warning: " << * i << " (" << i << ") was not deleted." << endl;
		}
		assert (Object :: objects . is_empty ());*/
	#endif

	cout << "The Scattered Lands is shut down." << endl;

	return 0;
}
