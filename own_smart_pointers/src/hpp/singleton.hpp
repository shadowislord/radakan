#ifndef RADAKAN_SINGLETON_HPP
#define RADAKAN_SINGLETON_HPP

#include "object.hpp"

#include <OgreSingleton.h>

using namespace std;

namespace Radakan
{

	///	Singleton is the base class for classes with only one instance.
	template <class T> class Singleton :
		public virtual Object,
		protected Ogre :: Singleton <T>
	{
		public :
			static string get_class_name ();

			static bool is_instantiated ();
			static Reference <T> get ();
			static void uninstantiate ();

			//	virtual class, constructor protected
			virtual ~Singleton ();
			
			virtual bool is_initialized () const;

		protected :
			Singleton ();
	};
}

#endif	//	RADAKAN_SINGLETON_HPP
