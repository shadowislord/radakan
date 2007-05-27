#ifndef TSL_MULTISLOT_HPP
#define TSL_MULTISLOT_HPP

#include "container.hpp"

using namespace std;

namespace TSL
{
	namespace Items
	{

		///	A Multislot is an item that can contain a limited number of items of a specific class.
		template <class T> class Multislot :
			public Container
		{
			public :
				//	The constructor is protected, see below.
				virtual ~Multislot ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				virtual bool add (Item & item);

				static Multislot <T> & create
				(
					string new_name,
					string new_mesh_name,
					Ogre :: Vector3 new_size,
					float new_mass,
					int new_max_size
				);

			protected :
				Multislot
				(
					string new_name,
					string new_mesh_name,
					Ogre :: Vector3 new_size,
					float new_mass,
					int new_max_size
				);
		};
	}
}

#endif	//	TSL_MULTISLOT_HPP
