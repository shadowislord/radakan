#ifndef RADAKAN_MESH_DATA_HPP
#define RADAKAN_MESH_DATA_HPP

#include "object.hpp"

using namespace std;

namespace Radakan
{
	
	class Mesh_Data :
		public Object
	{
		public :
			static string get_class_name ();

			Mesh_Data (string new_file_name);
			virtual ~Mesh_Data ();
			virtual bool is_initialized () const;

			const string file_name;

			Mathematics :: Quaternion default_orientation;

			///	Default: empty
			string material_file_name;

			///	Default: '1'
			float scale;
			
			///	'false' means that you can pass through it.
			///	Default: 'true'
			bool solid;

			///	Default: 'true'
			bool visible;
	};
}

#endif	// RADAKAN_MESH_DATA_HPP
