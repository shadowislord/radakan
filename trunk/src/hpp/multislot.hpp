#ifndef RADAKAN_MULTISLOT_HPP
#define RADAKAN_MULTISLOT_HPP

#include "container.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{

		///	A Multislot is an item that can contain a limited number of items of a specific class.
		template <class T> class Multislot :
			public Container
		{
			public :
				Multislot
				(
					string new_name,
					string new_mesh_name,
					Ogre :: Vector3 new_size,
					float new_mass,
					int new_max_size
				);
				virtual ~Multislot ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				virtual bool add (Reference <Item> item);
		};
	}
}

#endif	//	RADAKAN_MULTISLOT_HPP
