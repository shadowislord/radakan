#ifndef RADAKAN_STRATEGY_HPP
#define RADAKAN_STRATEGY_HPP

#include "resident.hpp"

using namespace std;

namespace Radakan
{
	namespace Strategies
	{
		///	I'm the abstract base class for the strategy pattern.
		///	For more information about the strategy pattern,
		///	see http://en.wikipedia.org/wiki/Strategy_pattern .
		///	Use me in combination with the Strategy_State_Machine class.
		template <class T, class U> class Strategy :
			public virtual Object
		{
			public :
				//	protected constructor, see below
				virtual ~Strategy ();
				virtual bool is_initialized () const;
				
				static string get_class_name ();

				///	Don't call this method directly,
				///	let it be called by Strategy_State_Machine.
				///	'transit (...)' returns the next active strategy state.
				virtual Reference <T> transit (Reference <U> message) = 0;
				
			protected :
				Strategy ();
		};
	}
}

#endif	//	RADAKAN_STRATEGY_HPP
