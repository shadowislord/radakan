#ifndef TSL_QUIT_STATE_HPP
#define TSL_QUIT_STATE_HPP

#include "algorithm.hpp"
#include "singleton.hpp"

using namespace std;

namespace tsl
{
	class TSL;

	class Quit_State :
		public Singleton <Quit_State>,
		public Algorithm <TSL>
	{
		public :
			Quit_State ();
			virtual ~Quit_State ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			
			virtual Algorithm <TSL> & transit (TSL & owner);
	
		private :
			//	Copies are not allowed.
			Quit_State (const Quit_State & menu_state);
	};
}

#endif	//	TSL_QUIT_STATE_HPP
