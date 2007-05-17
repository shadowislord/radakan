#ifndef TSL_SINGLETON_HPP
#define TSL_SINGLETON_HPP

#include <OgreSingleton.h>
#include "object.hpp"

using namespace std;

namespace TSL
{

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

			static void destruct ();

		protected :
			Singleton ();
	};
}

#endif	//	TSL_SINGLETON_HPP
