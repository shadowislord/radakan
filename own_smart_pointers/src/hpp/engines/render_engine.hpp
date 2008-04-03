#ifndef RADAKAN_ENGINES_RENDER_ENGINE_HPP
#define RADAKAN_ENGINES_RENDER_ENGINE_HPP

#include "singleton.hpp"

using namespace std;

namespace Ogre
{
	class Camera;
	class SceneManager;
}

namespace Radakan
{
	namespace Engines
	{
		class Render_Engine :
			public Singleton <Render_Engine>
		{
			public :
				static string get_class_name ();
				
				Render_Engine ();
				virtual ~Render_Engine ();
				virtual bool is_initialized () const;

				void render ();

				boost :: shared_ptr <Ogre :: RenderWindow> get_window ();
				boost :: shared_ptr <Ogre :: SceneManager> get_scene_manager ();

				void set_skydome (string skydome_name);

				float get_FPS () const;

			private :
				boost :: shared_ptr <Ogre :: Root> root;
				boost :: shared_ptr <Ogre :: RenderWindow> window;
				boost :: shared_ptr <Ogre :: SceneManager> scene_manager;
				boost :: scoped_ptr <Ogre :: Camera> camera;
				
				unsigned int turn;
				boost :: scoped_ptr <Ogre :: Timer> timer;
		};
	}
}

#endif	//	RADAKAN_ENGINES_RENDER_ENGINE_HPP
