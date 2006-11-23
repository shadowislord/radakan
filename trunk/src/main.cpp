#include "tslrpg.hpp"

using namespace std;
using namespace sl;

string path;
ostream * sl_out = & cout;

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
	
	* sl_out << "Setting up Scattered Lands..." << endl;

	#ifdef SL_WIN32
		path = strCmdLine;

		if (path . empty ())
		{
			TCHAR buffer [MAX_PATH];
			GetCurrentDirectory (MAX_PATH, buffer);
			path = buffer;
		}
	#else
		path = argv [argc - 1];
	#endif

	* sl_out << "path: " << path << endl;

	#ifdef SL_DEBUG
		* sl_out << "debug mode: enabled." << endl;

		sl_out = new ofstream ((path + "/log/log.txt") . c_str ());

		* sl_out << "Setting up Scattered Lands..." << endl;
		* sl_out << "path: " << path << endl;

		* sl_out << "debug mode: enabled." << endl;
	#else
		* sl_out << "debug mode: disabled." << endl;
	#endif

	try
	{
		Tslrpg game (path);
		* sl_out << "Scattered Lands is set up." << endl;

		* sl_out << "Running Scattered Lands..." << endl;
		game . run ();
		* sl_out << "Scattered Lands is stopped." << endl;

		* sl_out << "Shutting down Scattered Lands..." << endl;
		//	game is automatically shut down.
	}
	catch (Ogre :: Exception & e)
	{
		#ifdef SL_WIN32
			MessageBox (NULL, e . getFullDescription () . c_str (), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
			* sl_out << "ERROR: " << e . getFullDescription () << endl;
		#endif
	}

	#ifdef SL_DEBUG
		for (set <Object *> :: const_iterator i = objects . begin ();
													i != objects . end (); i ++)
		{
			* sl_out << "Warning: " << * * i << " was not deleted." << endl;
		}
		assert (objects . empty ());
	#endif

	* sl_out << "Scattered Lands is shut down." << endl;

	return 0;
}
