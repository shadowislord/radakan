#include "environment.hpp"

using namespace std;
using namespace tsl;

Environment	::
	Environment (Ogre :: SceneManager & scene_manager, Ogre :: Vector3 new_gravity) :
	Object ("environment"),
	OgreOde :: World (& scene_manager),
	root_node (* scene_manager . getRootSceneNode ())
{
	setGravity (new_gravity);

	setShowDebugGeometries (true);

	log (debugging) << "ERP: " << getERP () << endl;
	log (debugging) << "CFM: " << getCFM () << endl;

	//	TODO make the next line work.
	getSceneManager () -> setSkyDome (true, "Peaceful", 10, 5);
}

Environment ::
	~Environment ()
{
	log (debugging) << "~" << get_class_name () << " ()" << endl;
	assert (Environment :: is_initialized ());
}


//	virtual
bool Environment ::
	is_initialized ()
	const
{
	return warn <Environment> (Object :: is_initialized ());
}

//	static
string Environment ::
	get_class_name ()
{
	return "Environment";
}
