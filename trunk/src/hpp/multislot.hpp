#ifndef TSL_MULTISLOT_HPP
#define TSL_MULTISLOT_HPP

#include "container.hpp"

using namespace std;

namespace tsl
{

	///	A Multislot can contain a limited number of objects of a specific class.

	template <class T> class Multislot :
		public Container
	{
		public :
			//	The constructor is protected, see below.
			virtual ~Multislot ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			
			virtual bool add (Item & item);

			static Multislot <T> & create
			(
				string new_name,
				string mesh_name,
				float new_volume,
				float new_weight,
				unsigned int new_max
			);

			const unsigned int max;
			
		protected :
			Multislot
			(
				string new_name,
				string mesh_name,
				float new_volume,
				float new_weight,
				unsigned int new_max
			);

		private :
			///	Copies are not allowed.
			Multislot (const Multislot & multislot);
	};
}

#endif	//	TSL_MULTISLOT_HPP
