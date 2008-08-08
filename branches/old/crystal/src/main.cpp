//	Main entry point for tslrpg.

#include "tsl.hpp"

CS_IMPLEMENT_APPLICATION

vector <Object *> objects;

int main (int argc, char * * argv)
{
//	Runs the application.
//	
//	csApplicationRunner<> cares about creating an application instance
//	which will perform initialization and event handling for the entire game.
//	
//	The underlying csApplicationFramework also performs some core
//	initialization.  It will set up the configuration manager, event queue,
//	object registry, and much more.  The object registry is very important,
//	and it is stored in your main application class (again, by
//	csApplicationFramework).
	return csApplicationRunner <Tsl> :: Run (argc, argv);
}
