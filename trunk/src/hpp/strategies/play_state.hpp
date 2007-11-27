#ifndef RADAKAN_PLAY_STATE_HPP
#define RADAKAN_PLAY_STATE_HPP

#include "pointer.hpp"
#include "singleton.hpp"
#include "strategy.hpp"

using namespace std;

namespace Ogre
{
	class Camera;
	class SceneManager;
}

namespace Radakan
{
	class Play_State_GUI;
	
	namespace Engines
	{
		class Game;
	}
	
	namespace Items
	{
		class Character;
		class Item;
	}
	
	namespace Strategies
	{

		class Play_State :
			public Singleton <Play_State>,
			public Strategy <Engines :: Game>
		{
			public :
				static string get_class_name ();

				Play_State (boost :: shared_ptr <Ogre :: SceneManager> scene_manager);
				virtual ~Play_State ();
				virtual bool is_initialized () const;
				
				virtual Reference <Strategy <Engines :: Game> > transit
					(const Reference <Messages :: Message <Engines :: Game> > & message);

			private :
				Reference <Play_State_GUI> gui;
				
				boost :: scoped_ptr <Ogre :: Camera> camera;

				Pointer <Items :: Character> character_target;
				Pointer <Items :: Item> item_target;
		};
	}
}

#endif	//	RADAKAN_PLAY_STATE_HPP
