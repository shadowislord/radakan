#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	#include "bullet_body.hpp"
	#include "bullet_world.hpp"
	#include "engines/log.hpp"

	#include <btBulletDynamicsCommon.h>

	using namespace std;
	using namespace Radakan;

	//	static
	string Bullet_Body ::
		get_class_name ()
	{
		return "Bullet_Body";
	}

	//	static
	boost :: shared_ptr <btCollisionShape> Bullet_Body ::
		create_collsion_shape (Mathematics :: Vector_3D size)
	{
		if (size . y <= 0.01)
		{
			return boost :: shared_ptr <btCollisionShape>
				(new btStaticPlaneShape
					(Mathematics :: Vector_3D :: y_axis . to_bullet (), 0));
		}
		else
		{
			return boost :: shared_ptr <btCollisionShape>
				(new btBoxShape (size . to_bullet ()));
		}
	}

	//	static
	Mathematics :: Vector_3D Bullet_Body ::
		get_local_inertia (boost :: shared_ptr <btCollisionShape> shape, float mass)
	{
		btVector3 temp;
		shape -> calculateLocalInertia (mass, temp);

		return Mathematics :: Vector_3D (temp);
	}

	Bullet_Body ::
		Bullet_Body
	(
		float mass,
		Mathematics :: Vector_3D size,
		Mathematics :: Vector_3D position,
		Mathematics :: Quaternion orientation
	) :
	Object ("anonymous body"),
	motion_state
	(
		new btDefaultMotionState
		(
			btTransform
			(
				orientation . to_bullet (),
				position . to_bullet ()
			)
		)
	),
	collision_shape (create_collsion_shape (size)),
	body
	(
		new btRigidBody
		(
			btRigidBody :: btRigidBodyConstructionInfo
			(
				mass,
				motion_state . get (),
				collision_shape . get (),
				get_local_inertia (collision_shape, mass) . to_bullet ()
			)
		)
	)
	{
		Singleton <Bullet_World> :: get () -> add_body (body);
	}

	//	virtual
	Bullet_Body ::
		~Bullet_Body ()
	{
		assert (Body :: is_initialized ());

		//	Nothing to do.
	}

	//	virtual
	bool Bullet_Body ::
		is_initialized ()
		const
	{
		assert (Body :: is_initialized ());

		return true;
	}

	//	virtual
	Mathematics :: Vector_3D Bullet_Body ::
		get_position ()
		const
	{
		assert (Bullet_Body :: is_initialized ());

		btTransform transformation;
		motion_state -> getWorldTransform (transformation);

		return Mathematics :: Vector_3D (transformation . getOrigin ());
	}

	//	virtual
	Mathematics :: Quaternion Bullet_Body ::
		get_orientation ()
		const
	{
		assert (Bullet_Body :: is_initialized ());
		
		btTransform transformation;
		motion_state -> getWorldTransform (transformation);

		return Mathematics :: Quaternion (transformation . getRotation ());
	}

		//	virtual
	Mathematics :: Vector_3D Bullet_Body ::
		get_linear_velocity ()
		const
	{
		return body -> getLinearVelocity ();
	}

		//	virtual
	Mathematics :: Vector_3D Bullet_Body ::
		get_angular_velocity ()
		const
	{
		return body -> getAngularVelocity ();
	}

	//	virtual
	void Bullet_Body ::
		apply_force (Mathematics :: Vector_3D force)
	{
		body -> applyCentralForce (force . to_bullet ());
	}

	//	virtual
	void Bullet_Body ::
		apply_torque (Mathematics :: Vector_3D torque)
	{
		body -> applyTorque (torque . to_bullet ());
	}

	//	virtual
	void Bullet_Body ::
		reset ()
	{
		Engines :: Log :: trace (me, Bullet_Body :: get_class_name (), "reset");
		assert (Bullet_Body :: is_initialized ());

		//	body -> setOrientation (Mathematics :: Quaternion :: identity);
		body -> translate (2 * Mathematics :: Vector_3D :: y_axis . to_bullet ());
		apply_force (Mathematics :: Vector_3D :: zero_vector);
		apply_torque (Mathematics :: Vector_3D :: zero_vector);
	}

#endif
