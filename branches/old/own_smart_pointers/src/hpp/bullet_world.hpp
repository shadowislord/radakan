#ifndef RADAKAN_BULLET_WORLD_HPP
#define RADAKAN_BULLET_WORLD_HPP

#include "world.hpp"

class btBroadphaseInterface;
class btCollisionConfiguration;
class btConstraintSolver;
class btDiscreteDynamicsWorld;
class btDispatcher;
class btRigidBody;

namespace Radakan
{
	class Bullet_World :
		public Singleton <Bullet_World>,
		public World
	{
		public:
			static string get_class_name ();
			
			Bullet_World ();
			virtual ~Bullet_World ();
			virtual bool is_initialized () const;
			
			virtual void update ();

			void add_body (boost :: shared_ptr <btRigidBody> body);

		private :
			boost :: scoped_ptr <btCollisionConfiguration> collision_configuration;
			boost :: scoped_ptr <btDispatcher> dispatcher;
			boost :: scoped_ptr <btBroadphaseInterface> broadphase;
			boost :: scoped_ptr <btConstraintSolver> constraint_solver;

		public :
			boost :: scoped_ptr <btDiscreteDynamicsWorld> bullet_world;
	};
}

#endif	// RADAKAN_BULLET_WORLD_HPP
