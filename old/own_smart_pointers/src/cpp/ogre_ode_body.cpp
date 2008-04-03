#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	#include "engines/log.hpp"
	#include "items/item.hpp"
	#include "world.hpp"
	#include "engines/settings.hpp"

	#include <OgreSceneNode.h>

	#include <OgreOdeGeometry.h>
	#include <OgreOdeBody.h>
	#include <OgreOdeMass.h>

	using namespace std;
	using namespace Radakan;

	//	static
	string Manipulatable_Body ::
		get_class_name ()
	{
		return "Manipulatable_Body";
	}

	//  constructor
	Manipulatable_Body ::
		Manipulatable_Body (Reference <Items :: Item> new_item, Mathematics :: Vector_3D position) :
		Object (new_item . get_name () + "'s movable model"),
		Model (new_item, position)
	{
		Engines :: Log :: trace (me, Manipulatable_Body :: get_class_name (), "",
			new_item . get_name (), position . to_string ());

		body . reset (new OgreOde :: Body (World :: get () -> ogre_ode_world . get (), name));
		body -> setMass (OgreOde :: BoxMass (item -> mass, item -> size));
		geometry -> setBody (body . get ());
		node -> attachObject (body . get ());

		assert (Model :: is_initialized ());
	}

	//  destructor
	Manipulatable_Body ::
		~Manipulatable_Body ()
	{
		Engines :: Log :: trace (me, Manipulatable_Body :: get_class_name (), "~");
		assert (Model :: is_initialized ());

		prepare_for_destruction ();

		node -> detachObject (body . get ());
	}

	//	virtual
	bool Manipulatable_Body ::
		is_initialized ()
		const
	{
		assert (Model :: is_initialized ());

		return true;
	}

	//	virtual
	void Manipulatable_Body ::
		apply_force (Mathematics :: Vector_3D force)
	{
		body -> setForce (force);
	}

	//	virtual
	void Manipulatable_Body ::
		apply_torque (Mathematics :: Vector_3D torque)
	{
		body -> setTorque (torque);
	}

	//	virtual
	void Manipulatable_Body ::
		reset ()
	{
		Engines :: Log :: trace (me, Manipulatable_Body :: get_class_name (), "reset");
		assert (Model :: is_initialized ());

		body -> setOrientation (Mathematics :: Quaternion :: identity);
		body -> setPosition (body -> getPosition ()
			+ (2 - body -> getPosition () . y) * Mathematics :: Vector_3D :: y_axis);
		body -> setLinearVelocity (Mathematics :: Vector_3D :: zero_vector);
		body -> setForce (Mathematics :: Vector_3D :: zero_vector);
	}



	#include "bullet_body.hpp"

	using namespace std;
	using namespace Radakan;

	geometry (create_geometry (item)),

	World :: get () -> bullet_world -> addRigidBody (body . get ());

	if (item . is_castable <Items :: Static_Item> ())
	{
		Engines :: Log :: log (me) << "I'm a static model." << endl;

		geometry -> setUserObject (entity . get ());
		geometry -> setPosition (position);
	}

	if (! item -> mesh_data -> solid)
	{
		geometry -> disable ();
	}


	//	static
	boost :: shared_ptr <OgreOde :: Geometry> Bullet_Body ::
		create_geometry (Reference <Items :: Item> item)
	{
		if (size . y <= 0.01)
		{
			return boost :: shared_ptr <OgreOde :: Geometry>
			(
				new OgreOde :: InfinitePlaneGeometry
				(
					Ogre :: Plane (Mathematics :: Vector_3D :: y_axis, 0),
					World :: get () -> ogre_ode_world . get ()
				)
			);
		}
		else
		{
			return boost :: shared_ptr <OgreOde :: Geometry>
			(
				new OgreOde :: BoxGeometry
					(size, World :: get () -> ogre_ode_world . get ())
			);
		}
	}

	Mathematics :: Vector_3D Bullet_Body ::
		get_position () const
	{
		assert (Model :: is_initialized ());

		btTransform transformation;
		motion_state -> getWorldTransform (transformation);

		return Mathematics :: Vector_3D (transformation . getOrigin ());
	}

	Mathematics :: Quaternion Bullet_Body ::
		get_orientation () const
	{
		assert (Model :: is_initialized ());
		
		btTransform transformation;
		motion_state -> getWorldTransform (transformation);

		return Mathematics :: Quaternion (transformation . getRotation ());
	}

	void Bullet_Body ::
		set_space (boost :: shared_ptr <OgreOde :: Space> new_space)
	{
		/*boost :: shared_ptr <OgreOde :: Space> old_space (geometry -> getSpace ());
		if (old_space)
		{
			old_space -> removeGeometry (* geometry . get ());
		}*/
		new_space -> addGeometry (* geometry . get ());
	}
#endif
