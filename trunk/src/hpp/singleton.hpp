#ifndef TSL_SINGLETON_HPP
#define TSL_SINGLETON_HPP

#include <OgreSingleton.h>
#include "object.hpp"

using namespace std;

namespace tsl
{

	template <class T> class Singleton :
		public virtual Object,
		protected Ogre :: Singleton <T>
	{
		public :
			//	virtual class, constructor protected
			virtual ~Singleton ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			static T & get ();
			static bool is_instantiated ();

		protected:
			Singleton ();
	};
}

#endif	//	TSL_SINGLETON_HPP
