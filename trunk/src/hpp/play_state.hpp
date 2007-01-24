#ifndef TSL_PLAY_STATE_HPP
#define TSL_PLAY_STATE_HPP

#include "algorithm.hpp"
#include "data_state_machine.hpp"
#include "sector.hpp"
#include "gui.hpp"

using namespace std;

namespace tsl
{
	class TSL;

	class Play_State :
		public Singleton <Play_State>,
		public Algorithm <TSL>,
		public Data_State_Machine <Sector>
	{
		public :
			Play_State ();
			virtual ~Play_State ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			
			virtual void set_active_state (Sector & new_sector);
			virtual Algorithm <TSL> & transit (TSL & owner);
	
		private :
			//	Copies are not allowed.
			Play_State (const Play_State & play_state);
			
			GUI & gui;
	};
}

#endif	//	TSL_PLAY_STATE_HPP
