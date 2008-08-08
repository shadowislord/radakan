#include "tslrpg.hpp"

using namespace std;

#ifdef SL_DEBUG
	ofstream * log_cout;

	set <Object *> objects;
#else
	//	This is faster, I think.
	//	Is it possible to 'absorb' this?
	ostream * log_cout = & cout;
#endif

#ifdef SL_WIN32
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

		#ifdef SL_WIN32
			string path = strCmdLine;

			if (path == "")
			{
				TCHAR buffer [MAX_PATH];
				GetCurrentDirectory (MAX_PATH, buffer);
				path = buffer;
			}
		#else
			string path = argv [argc - 1];
		#endif

		cout << "path: " << path << endl;

		#ifdef SL_DEBUG
			log_cout = new ofstream ((path + "/logs/log.txt").c_str ());

			* log_cout << "Setting up Scattered Lands..." << endl;
			* log_cout << "path: " << path << endl;

			* log_cout << "debug mode: enabled." << endl;
		#else
			* log_cout << "debug mode: disabled." << endl;
		#endif

		game = new Tslrpg (path);
		* log_cout << "Scattered Lands is set up." << endl;

		* log_cout << "Running Scattered Lands..." << endl;
		game->run ();
		* log_cout << "Scattered Lands is stopped." << endl;

		* log_cout << "Shutting down Scattered Lands..." << endl;
		delete game;

		#ifdef SL_DEBUG
			for (set <Object *> :: const_iterator i = objects.begin ();
													i != objects.end (); i ++)
			{
				* log_cout << "Not deleted: " << * * i << endl;
			}
			assert (objects.empty ());
		#endif

		* log_cout << "Scattered Lands is shut down." << endl;
	}
	catch (Ogre :: Exception & e)
	{
		#ifdef SL_WIN32
			MessageBox (NULL, e.getFullDescription ().c_str (), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
			cerr << "An exception has occured: " << e.getFullDescription () << endl;
		#endif
	}

	return 0;
}
