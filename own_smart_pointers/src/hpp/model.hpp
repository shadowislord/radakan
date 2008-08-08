#ifndef RADAKAN_MODEL_HPP
#define RADAKAN_MODEL_HPP

#include "object.hpp"

using namespace std;

namespace Ogre
{
	class SceneNode;
}

namespace Radakan
{
	class Mesh_Data;

	///	'Model' is a 3D representation of an 'Items :: Item'.
	class Model :
		public Object
	{
		public :
			static string get_class_name ();

			///	Call this constructor through 'Items :: Item :: appear (...)'.
			Model
			(
				Reference <Mesh_Data> mesh_data,
				Mathematics :: Vector_3D position,
				Mathematics :: Quaternion orientation,
				Mathematics :: Vector_3D bounding_box	//	For debugging
			);
			virtual ~Model ();
			virtual bool is_initialized () const;

			void set_position (Mathematics :: Vector_3D position);
			void set_orientation (Mathematics :: Quaternion orientation);
			
			///	Default: 'true'
			bool visible;
			
		private :
			Mathematics :: Quaternion default_orientation;
			
			boost :: shared_ptr <Ogre :: SceneNode> node;
			boost :: scoped_ptr <Ogre :: Entity> entity;
	};
}

#endif	//	RADAKAN_MODEL_HPP
