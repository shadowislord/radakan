#ifndef RADAKAN_BULLET_BODY_HPP
#define RADAKAN_BULLET_BODY_HPP
	#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE

	#include "body.hpp"

	using namespace std;

	class btMotionState;
	class btCollisionShape;
	class btRigidBody;
		
	namespace Radakan
	{
		
		class Bullet_Body :
			public Body
		{
			public :
				static string get_class_name ();
				
			private :
				static boost :: shared_ptr <btCollisionShape> create_collsion_shape
					(Mathematics :: Vector_3D size);
				static Mathematics :: Vector_3D get_local_inertia
					(boost :: shared_ptr <btCollisionShape> shape, float mass);
				
			public :
				Bullet_Body
				(
					float mass,
					Mathematics :: Vector_3D size,
					Mathematics :: Vector_3D position,
					Mathematics :: Quaternion orientation
				);
				virtual ~Bullet_Body ();
				virtual bool is_initialized () const;
				
				virtual Mathematics :: Vector_3D get_position () const;
				virtual Mathematics :: Quaternion get_orientation () const;
				
				virtual Mathematics :: Vector_3D get_linear_velocity () const;
				virtual Mathematics :: Vector_3D get_angular_velocity () const;
				
				virtual void apply_force (Mathematics :: Vector_3D force);
				virtual void apply_torque (Mathematics :: Vector_3D torque);
				
				///	Reset the position and orientation.
				virtual void reset ();
				
			private :
				boost :: scoped_ptr <btMotionState> motion_state;
				boost :: shared_ptr <btCollisionShape> collision_shape;
				boost :: shared_ptr <btRigidBody> body;
		};
	}

	#endif
#endif	//	RADAKAN_BULLET_BODY_HPP
