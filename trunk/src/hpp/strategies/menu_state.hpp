#ifndef RADAKAN_MENU_STATE_HPP
#define RADAKAN_MENU_STATE_HPP

#include "strategy.hpp"
#include "singleton.hpp"

using namespace std;

namespace Radakan
{
	class GUI;

	namespace Engines
	{
		class Game;
	}

	namespace Strategies
	{
		///	Menu_State is the strategy for the game when displaying the menu.
		class Menu_State :
			public Singleton <Menu_State>,
			public Strategy <Engines :: Game>
		{
			public :
				Menu_State ();
				virtual ~Menu_State ();
				virtual bool is_initialized () const;
				
				static string get_class_name ();
				
				virtual Reference <Strategy <Engines :: Game> > transit
					(const Reference <Messages :: Message <Engines :: Game> > & message);
		
			private :
				Reference <GUI> gui;
		};
	}
}

#endif	//	RADAKAN_MENU_STATE_HPP
