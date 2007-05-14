#ifndef TSL_ALGORITHM_HPP
#define TSL_ALGORITHM_HPP

#include "singleton.hpp"

using namespace std;

namespace TSL
{

	///	Algorithm is the abstract base class for all algorithms.

	template <class T> class Algorithm :
		public virtual Object
	{
		public :
			//	protected constructor, see below
			virtual ~Algorithm ();
			virtual bool is_initialized () const;
			
			static const string class_name;

			virtual Algorithm <T> & transit (T & owner);

			///	First recursively transit the anchestors and then me.
			Algorithm <T> & full_transit (T & owner);

			virtual void enter (T & owner);
			virtual void exit (T & owner);

		protected :
			Algorithm (Algorithm <T> * new_parent = NULL);

		private :
			Algorithm <T> * parent;
	};
}

#endif	//	TSL_ALGORITHM_HPP
