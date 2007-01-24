#ifndef TSL_STATE_HPP
#define TSL_STATE_HPP

#include "singleton.hpp"

using namespace std;

namespace tsl
{

	///	Algorithm is the abstract base class for all algorithms.

	template <class T> class Algorithm :
		public virtual Object
	{
		public :
			//	protected constructor, see below
			virtual ~Algorithm ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			///	pure virtual method
			virtual Algorithm <T> & transit (T & owner) = 0;

		protected:
			Algorithm ();
	};
}

#endif	//	TSL_STATE_HPP
