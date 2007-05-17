#include "environment.hpp"
#include "log.hpp"

using namespace std;
using namespace TSL;

//	static
const string Environment ::
	get_class_name ()
{
	return "Environment";
}

Environment	::
	Environment (Ogre :: SceneManager & scene_manager, Ogre :: Vector3 new_gravity) :
	Object ("environment"),
	OgreOde :: World (& scene_manager),
	root_node (* scene_manager . getRootSceneNode ())
{
	setGravity (new_gravity);

	//	The following line causes an error in Ogre.
	//	setShowDebugGeometries (true);

	Log :: log (me) << "ERP: " << getERP () << endl;
	Log :: log (me) << "CFM: " << getCFM () << endl;

	//	TODO make the next line work.
	//	getSceneManager () -> setSkyDome (true, "Peaceful", 10, 5);
}

Environment ::
	~Environment ()
{
	Log :: trace <Environment> (me, "~");
	assert (Environment :: is_initialized ());
}


//	virtual
bool Environment ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}
