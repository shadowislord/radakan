#include "tslrpg.hpp"

using namespace std;
using namespace sl;

#ifdef SL_DEBUG
	set <Object *> sl :: objects;
#endif

#ifdef SL_WIN32
	INT WINAPI WinMain (HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main (int argc, char * argv [])
#endif
{
	//	check for conflicting debug and trace flags
	#ifdef SL_TRACE
		#ifndef SL_DEBUG
			abort ();
		#endif
	#endif

	cout << "Setting up Scattered Lands..." << endl;

	string sl_path;
	string ogre_path;
	#ifdef SL_WIN32
		//	!!! This is incorrectely handled.
		sl_path = strCmdLine;

		if (sl_path . empty ())
		{
			TCHAR buffer [MAX_PATH];
			GetCurrentDirectory (MAX_PATH, buffer);
			sl_path = buffer;
		}
		else
		{
			int spacePos = 0;
			for (int i = 0;i < sl_path.length();i++)
			{
				if (sl_path[i] == '~')
				{
					spacePos = i;
				}

			}
			ogre_path = sl_path.substr(spacePos+1,sl_path.length());
            sl_path = sl_path.substr(0,spacePos);
			clog << "SLPATH:\t" << sl_path << endl;
			clog << "OGPATH:\t" << ogre_path << endl;
		}
	#else
		sl_path = argv [argc - 2];
		ogre_path = argv [argc - 1];
	#endif

	#ifdef SL_DEBUG
		cout << "debug mode: enabled." << endl;
		#ifdef SL_TRACE
			cout << "trace mode: enabled." << endl;
		#else
			cout << "trace mode: disabled." << endl;
		#endif

		//	'cout' is redirected to a log file.
		//	Note: don't use 'cout' or 'cerr'.
		cout . rdbuf ((new ofstream ((sl_path + "/log/log.txt") . c_str ())) -> rdbuf ());

		cout << "debug mode: enabled." << endl;

		cout << "Setting up Scattered Lands..." << endl;
		cout << "sl_path: " << sl_path << endl;
		cout << "ogre_path: " << ogre_path << endl;
	#else
		cout << "debug mode: disabled." << endl;

		//	From here on, all cout messages are ignored.
		cout . rdbuf ((new ostream (new stringbuf (ios_base :: out))) -> rdbuf ());
	#endif

	try
	{
		Tslrpg game (sl_path, ogre_path);
		cout << "Scattered Lands is set up." << endl;

		cout << "Running Scattered Lands..." << endl;
		game . run ();
		cout << "Scattered Lands is stopped." << endl;

		cout << "Shutting down Scattered Lands..." << endl;
		//	The game is automatically shut down here.
	}
	catch (Ogre :: Exception & e)
	{
		#ifdef SL_WIN32
			MessageBox (NULL, e . getFullDescription () . c_str (), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
			cerr << "ERROR: " << e . getFullDescription () << endl;
		#endif
	}

	#ifdef SL_DEBUG
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
