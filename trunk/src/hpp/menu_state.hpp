#ifndef TSL_MENU_STATE_HPP
#define TSL_MENU_STATE_HPP

#include "algorithm.hpp"
#include "singleton.hpp"

using namespace std;

namespace TSL
{
	class GUI;

	namespace Engines
	{
		class Game;
	}

	namespace Algorithms
	{
		///	Menu_State is the algorithm for the game when displaying the menu.
		class Menu_State :
			public Singleton <Menu_State>,
			public Algorithm
		{
			public :
				Menu_State ();
				virtual ~Menu_State ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				virtual Algorithm * transit (const Object & message);
		
			private :
				GUI & gui;
		};
	}
}

#endif	//	TSL_MENU_STATE_HPP
