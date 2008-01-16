#ifndef RADAKAN_STRATEGIES_GAME_MODES_PLAY_HPP
#define RADAKAN_STRATEGIES_GAME_MODES_PLAY_HPP

#include "pointer.hpp"
#include "singleton.hpp"
#include "strategies/game_modes/game_mode.hpp"

using namespace std;

namespace Ogre
{
	class Camera;
	class SceneManager;
}

namespace Radakan
{
	namespace Items
	{
		class Item;

		namespace Characters
		{
			class Character;
		}
	}
	
	namespace Strategies
	{
		namespace Game_Modes
		{
			class Play :
				public Singleton <Play>,
				public Game_Mode
			{
				public :
					static string get_class_name ();

					Play
					(
						boost :: shared_ptr <Ogre :: RenderWindow> window,
						boost :: shared_ptr <Ogre :: SceneManager> scene_manager
					);
					virtual ~Play ();
					virtual bool is_initialized () const;
					
					virtual Reference <Game_Mode> transit
						(const Reference <Messages :: Message <Object> > & message);

				private :
					boost :: scoped_ptr <Ogre :: Camera> camera;

					Pointer <Items :: Characters :: Character> character_target;
					Pointer <Items :: Item> item_target;
			};
		}
	}
}

#endif	//	RADAKAN_STRATEGIES_GAME_MODES_PLAY_HPP
