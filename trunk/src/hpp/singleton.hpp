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
			
			static string get_class_name ();

			static void destruct ();

			static Reference <T> get ();
			static bool is_instantiated ();

		protected :
			Singleton ();

		private :
			///	Store an extra reference to make sure Singletons are not destructed.
			static Reference <T> myself;
	};
}

#endif	//	RADAKAN_SINGLETON_HPP
