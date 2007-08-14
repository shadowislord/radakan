#ifndef RADAKAN_SINGLETON_HPP
#define RADAKAN_SINGLETON_HPP

#include <OgreSingleton.h>
#include "object.hpp"

using namespace std;

namespace Radakan
{

	///	Singleton is the base class for classes with only one instance.
	template <class T> class Singleton :
		public virtual Object,
		protected Ogre :: Singleton <T>
	{
		public :
			//	virtual class, constructor protected
			virtual ~Singleton ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();

			static T & get ();
			static bool is_instantiated ();

		protected :
			Singleton ();
	};
}

#endif	//	RADAKAN_SINGLETON_HPP
