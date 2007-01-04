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

	clog << "Setting up Scattered Lands..." << endl;

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
	#else
		sl_path = argv [argc - 2];
		ogre_path = argv [argc - 1];
	#endif

	#ifdef SL_DEBUG
		clog << "debug mode: enabled." << endl;
		
		//	'clog' is redirected to a log file.
		//	Note: don't use 'cout' or 'cerr'.
		clog . rdbuf ((new ofstream ((sl_path + "/log/log.txt") . c_str ())) -> rdbuf ());

		clog << "debug mode: enabled." << endl;

		clog << "Setting up Scattered Lands..." << endl;
		clog << "sl_path: " << sl_path << endl;
		clog << "ogre_path: " << ogre_path << endl;
	#else
		clog << "debug mode: disabled." << endl;
	#endif

	try
	{
		Tslrpg game (sl_path, ogre_path);
		clog << "Scattered Lands is set up." << endl;

		clog << "Running Scattered Lands..." << endl;
		game . run ();
		clog << "Scattered Lands is stopped." << endl;

		clog << "Shutting down Scattered Lands..." << endl;
		//	The game is automatically shut down here.
	}
	catch (Ogre :: Exception & e)
	{
		#ifdef SL_WIN32
			MessageBox (NULL, e . getFullDescription () . c_str (), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
			clog << "ERROR: " << e . getFullDescription () << endl;
		#endif
	}

	#ifdef SL_DEBUG
		for (set <Object *> :: const_iterator i = objects . begin ();
													i != objects . end (); i ++)
		{
			clog << "Warning: " << * * i << " (" << * i << ") was not deleted." << endl;
		}
		assert (objects . empty ());
	#endif

	clog << "Scattered Lands is shut down." << endl;

	return 0;
}
