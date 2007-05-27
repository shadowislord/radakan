#ifndef TSL_MENU_STATE_HPP
#define TSL_MENU_STATE_HPP

#include "gui.hpp"

using namespace std;

namespace TSL
{
	namespace Engines
	{
		class Game;
	}

	namespace Algorithms
	{
		///	Menu_State is the algorithm for the game when displaying the menu.
		class Menu_State :
			public Singleton <Menu_State>,
			public Algorithm <Engines :: Game>
		{
			public :
				Menu_State ();
				virtual ~Menu_State ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				virtual Algorithm <Engines :: Game> & transit (Engines :: Game & owner, const Object & message);
		
			private :
				virtual void enter (Engines :: Game & owner);
				
				GUI & gui;
		};
	}
}

#endif	//	TSL_MENU_STATE_HPP
