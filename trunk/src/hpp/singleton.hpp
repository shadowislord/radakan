#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include "object.hpp"
#include <OgreSingleton.h>

using namespace std;

namespace tsl
{

	template <typename T> class Singleton :
		public virtual Object,
		protected Ogre :: Singleton <T>
	{
		public :
			//	virtual class, constructor protected
			virtual ~Singleton ();
			virtual bool is_initialized () const;
			static string get_type_name ();

			static T & get ();
			static bool is_instantiated ();

		protected:
			Singleton (string new_name);
	};
}

#endif	//	SINGLETON_HPP
