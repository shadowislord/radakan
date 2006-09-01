#include "tslrpg.hpp"

using namespace std;

#ifdef SL_DEBUG
	ofstream log_cout ("log.txt");

	set <Object *> objects;
#else
	//	This is faster.
	//	Is it possible to 'absorb' this?
	ofstream log_cout = cout;
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain (HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main (int argc, char * argv [])
#endif
{
	try
	{

		// Create application object
		Tslrpg * game;
		
		log_cout << "Setting up Scattered Lands..." << endl;
		game = new Tslrpg ();
		log_cout << "Scattered Lands is set up." << endl;
		
		log_cout << "Running Scattered Lands..." << endl;
		game->run ();
		log_cout << "Scattered Lands is stopped." << endl;
		
		log_cout << "Shutting down Scattered Lands..." << endl;
		delete game;

		#ifdef SL_DEBUG
			for (set <Object *> :: const_iterator i = objects.begin ();
													i != objects.end (); i ++)
			{
				log_cout << "Not deleted: " << * * i << endl;
			}
			assert (objects.empty ());
		#endif
	
		log_cout << "Scattered Lands is shut down." << endl;
	}
	catch (Exception & e)
	{
		#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox (NULL, e.getFullDescription ().c_str (), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
			cerr << "An exception has occured: " << e.getFullDescription () << endl;
		#endif
	}

	return 0;
}
