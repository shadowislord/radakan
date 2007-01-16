#include "tsl.hpp"

using namespace std;
using namespace tsl;

#ifdef TSL_DEBUG
	set <Object *> tsl :: objects;
#endif

#ifdef TSL_WIN32
	INT WINAPI WinMain (HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main (int argc, char * argv [])
#endif
{
	//	check for conflicting debug and trace flags
	#ifdef TSL_TRACE
		#ifndef TSL_DEBUG
			error () << " detected conflicting TSL_DEBUG and TSL_TRACE flags." << endl;
			abort ();
		#endif
	#endif

	cout << "Setting up Scattered Lands..." << endl;

	string tsl_path;
	string ogre_path;
	#ifdef TSL_WIN32
		tsl_path = strCmdLine;

		if (tsl_path . empty ())
		{
			TCHAR buffer [MAX_PATH];
			GetCurrentDirectory (MAX_PATH, buffer);
			tsl_path = buffer;
		}
		else
		{
			//	example arguments: C:/John/TSL~C:/OgreSDK
			int spacePos = tsl_path . find ('~');
			assert (spacePos != string :: npos);
			ogre_path = tsl_path . substr (spacePos + 1, tsl_path . length ());
            tsl_path = tsl_path . substr (0, spacePos);
		}
	#else
		//	example arguments: /home/john/tsl /usr/share/doc/ogre-1.2.4
		tsl_path = argv [argc - 2];
		ogre_path = argv [argc - 1];
	#endif

	#ifdef TSL_DEBUG
		cout << "debug mode: enabled." << endl;
		#ifdef TSL_TRACE
			cout << "trace mode: enabled." << endl;
		#else
			cout << "trace mode: disabled." << endl;
		#endif

		//	'cout' is redirected to a log file.
		//	Note: don't use 'cout' or 'cerr'.
		cout . rdbuf ((new ofstream ((tsl_path + "/log/log.txt") . c_str ())) -> rdbuf ());

		cout << "Setting up Scattered Lands..." << endl;
		cout << "tsl_path: " << tsl_path << endl;
		cout << "ogre_path: " << ogre_path << endl;
	#else
		cout << "debug mode: disabled." << endl;

		//	From here on, all cout messages are ignored.
		cout . rdbuf ((new ostream (new stringbuf (ios_base :: out))) -> rdbuf ());
	#endif

	try
	{
		TSL game (tsl_path, ogre_path);
		cout << "Scattered Lands is set up." << endl;

		cout << "Running Scattered Lands..." << endl;
		while (game . run () != game . quit)
		{}
		cout << "Scattered Lands is stopped." << endl;

		cout << "Shutting down Scattered Lands..." << endl;
		//	The game is automatically shut down here.
	}
	catch (Ogre :: Exception & e)
	{
		#ifdef TSL_WIN32
			MessageBox (NULL, e . getFullDescription () . c_str (), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
			cerr << "ERROR: " << e . getFullDescription () << endl;
		#endif
	}

	#ifdef TSL_DEBUG
		for (set <Object *> :: const_iterator i = objects . begin ();
													i != objects . end (); i ++)
		{
			cout << "Warning: " << * * i << " (" << * i << ") was not deleted." << endl;
		}
		assert (objects . empty ());
	#endif

	cout << "Scattered Lands is shut down." << endl;

	return 0;
}
