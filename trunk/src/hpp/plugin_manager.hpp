#ifndef TSL_PLUGIN_MANAGER_HPP
#define TSL_PLUGIN_MANAGER_HPP

#include "observer.hpp"
#include "play_state.hpp"
#include "singleton.hpp"

using namespace std;

namespace TSL
{
	namespace Engines
	{

		///	Plugin_Manager manages all plugins.
		class Plugin_Manager :
			public Singleton <Plugin_Manager>
		{
			public :
				Plugin_Manager ();
				virtual ~Plugin_Manager ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

			private :
				Set <Observer <Strategies :: Play_State> > plugins;
		};
	}
}

#endif	// TSL_PLUGIN_MANAGER_HPP
