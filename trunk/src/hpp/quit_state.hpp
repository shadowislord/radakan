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

			using Algorithm <TSL> :: enter;
			using Algorithm <TSL> :: exit;
	};
}

#endif	//	TSL_QUIT_STATE_HPP
