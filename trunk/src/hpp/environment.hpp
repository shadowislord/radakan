#ifndef TSL_ENVIRONMENT_HPP
#define TSL_ENVIRONMENT_HPP

#include "singleton.hpp"

#include <OgreSceneManager.h>

#include <OgreOde_Core.h>

namespace tsl
{
	class Environment :
		public Singleton <Environment>,
		//	public virtual Object,
		public OgreOde :: World
	{
		public:
			Environment (Ogre :: SceneManager & scene_manager, Ogre :: Vector3 new_gravity);
			virtual ~Environment ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			Ogre :: SceneNode & root_node;
	};
}

#endif	//	TSL_ENVIRONMENT_HPP
