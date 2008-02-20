#include "engines/log.hpp"
#include "engines/render_engine.hpp"
#include "items/static_item.hpp"
#include "model.hpp"
#include "slot.hpp"
#include "world.hpp"

#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreManualObject.h>

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	#include <OgreOdeGeometry.h>
	#include <OgreOdeSpace.h>
	#include <OgreOdeBody.h>
	#include <OgreOdeWorld.h>
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	#include <btBulletDynamicsCommon.h>
	#include <btBulletDynamicsCommon.h>
#else
#endif

using namespace std;
using namespace Radakan;

//	static
string Model ::
	get_class_name ()
{
	return "Model";
}

//  constructor
Model ::
	Model
	(
		Reference <Mesh_Data> mesh_data,
		Mathematics :: Vector_3D position,
		Mathematics :: Quaternion orientation,
		Mathematics :: Vector_3D bouding_box
	) :
	Object (mesh_data . get_name () + "'s model"),
	default_orientation (mesh_data -> default_orientation),
	node (World :: get () -> root_node -> createChildSceneNode (name)),
	entity
	(
		Engines :: Render_Engine :: get () -> get_scene_manager () -> createEntity
		(
			name + "'s entity", mesh_data -> file_name
		)
	)
{
	Engines :: Log :: trace (me, Model :: get_class_name (), "", mesh_data . get_name (),
		position . to_string (), orientation . to_string ());

//	World :: get () . root_node . addChild (node :: get ());

	set_position (position);
	set_orientation (orientation * default_orientation);
	node -> setScale (mesh_data -> scale, mesh_data -> scale, mesh_data -> scale);
	node -> attachObject (entity . get ());

	assert (node -> numAttachedObjects () == 1);
	
	if (! mesh_data -> material_file_name . empty ())
	{
		entity -> setMaterialName (mesh_data -> material_file_name);
	}

	Ogre :: ManualObject * box = Engines :: Render_Engine :: get ()
		-> get_scene_manager () -> createManualObject (me . get_name () + "'s box");
	
	box -> begin ("BaseWhiteNoLighting", Ogre :: RenderOperation :: OT_LINE_STRIP);
	
	box -> position (0,					0,					0);
	box -> position (0,					0,					bouding_box . z);
	box -> position (0,					bouding_box . y,	0);
	box -> position (0,					bouding_box . y,	bouding_box . z);
	box -> position (bouding_box . x,	0,					0);
	box -> position (bouding_box . x,	0,					bouding_box . z);
	box -> position (bouding_box . x,	bouding_box . y,	0);
	box -> position (bouding_box . x,	bouding_box . y,	bouding_box . z);
	
	box -> index (0);
	box -> index (1);
	box -> index (3);
	box -> index (2);
	box -> index (0);
	box -> index (4);
	box -> index (5);
	box -> index (7);
	box -> index (6);
	box -> index (4);
	box -> index (5);
	box -> index (1);
	box -> index (3);
	box -> index (7);
	box -> index (6);
	box -> index (2);
	
	box -> end ();
	
	node -> attachObject (box);
	
	assert (Model :: is_initialized ());
}

//  destructor
Model ::
	~Model ()
{
	Engines :: Log :: trace (me, Model :: get_class_name (), "~");
	assert (Model :: is_initialized ());

	prepare_for_destruction ();

	//	There's no need to remove myself from my item.

	assert (node -> numAttachedObjects () == 1);

	Engines :: Render_Engine :: get () -> get_scene_manager ()
		-> destroyMovableObject (entity . get ());

	assert (node -> numAttachedObjects () == 0);

	node -> getParent () -> removeChild (node -> getName ());
}

//	virtual
bool Model ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());
	assert (node -> getParent () != NULL);
	assert (World :: get () -> root_node . get () != NULL);
	
	assert (node -> getParent () == World :: get () -> root_node . get ());
///	assert (node -> numAttachedObjects () <= 2);

	return true;
}

void Model ::
	set_position (Mathematics :: Vector_3D position)
{
	assert (is_initialized ());

	node -> setPosition (position);
}

void Model ::
	set_orientation (Mathematics :: Quaternion orientation)
{
	assert (is_initialized ());
	
	node -> setOrientation (orientation * default_orientation);
}
