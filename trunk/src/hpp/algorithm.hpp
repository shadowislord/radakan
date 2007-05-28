#ifndef TSL_ALGORITHM_HPP
#define TSL_ALGORITHM_HPP

#include "singleton.hpp"

using namespace std;

namespace TSL
{
	namespace Algorithms
	{

		///	Algorithm is the abstract base class for all algorithms.
		template <class T> class Algorithm :
			public virtual Object
		{
			public :
				//	protected constructor, see below
				virtual ~Algorithm ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				virtual Algorithm <T> & transit (T & owner, const Object & message) = 0;

				virtual void enter (T & owner);

				virtual void exit (T & owner);

				///	First recursively transit my anchestors and then me.
				///	But when an anchestor doesn't return me, return that result instead.
				Algorithm <T> & recursive_transit (T & owner, const Object & message);

				void recursive_enter (T & owner);
				void recursive_exit (T & owner);

			protected :
				Algorithm ();
				Algorithm (Algorithm <T> & new_parent);

			private :
				Algorithm <T> & parent;
		};
	}
}

#endif	//	TSL_ALGORITHM_HPP
