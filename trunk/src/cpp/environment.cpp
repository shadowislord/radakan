#include "environment.hpp"

using namespace std;
using namespace TSL;

//	static
const string Environment ::
	class_name ("Environment");

Environment	::
	Environment (Ogre :: SceneManager & scene_manager, Ogre :: Vector3 new_gravity) :
	Object ("environment"),
	OgreOde :: World (& scene_manager),
	root_node (* scene_manager . getRootSceneNode ())
{
	setGravity (new_gravity);

	//	The following line causes an error in Ogre.
	//	setShowDebugGeometries (true);

	log (debugging) << "ERP: " << getERP () << endl;
	log (debugging) << "CFM: " << getCFM () << endl;

	//	TODO make the next line work.
	//	getSceneManager () -> setSkyDome (true, "Peaceful", 10, 5);
}

Environment ::
	~Environment ()
{
	log (debugging) << "~" << class_name << " ()" << endl;
	assert (Environment :: is_initialized ());
}


//	virtual
bool Environment ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}
