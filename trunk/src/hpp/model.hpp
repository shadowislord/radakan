#ifndef RADAKAN_MODEL_HPP
#define RADAKAN_MODEL_HPP

#include "location.hpp"
#include "resident.hpp"

using namespace std;

namespace Ogre
{
	class SceneNode;
}

namespace OgreOde
{
	class Geometry;
	class Space;
}

namespace Radakan
{
	namespace Items
	{
		class Item;
	}

	///	Model is a 3D representation of an Items :: Item.
	class Model :
		public Resident <Model>,
		public Location <Items :: Item>
	{
		public :
			Model (Reference <Items :: Item> new_item, Ogre :: Vector3 position, float scale);
			virtual ~Model ();
			virtual bool is_initialized () const;
			
			static string get_class_name ();
			
			Ogre :: Vector3 get_front_direction () const;
			Ogre :: Vector3 get_side_direction () const;
			Ogre :: Vector3 get_top_direction () const;

			void set_scale (float scale);
			void set_material (string name);

			void set_space (boost :: shared_ptr <OgreOde :: Space> new_space);

			Reference <Items :: Item> item;
			boost :: shared_ptr <Ogre :: SceneNode> node;

		protected :
			boost :: shared_ptr <OgreOde :: Geometry> geometry;
			boost :: shared_ptr <Ogre :: Entity> entity;
	};

	Ogre :: Quaternion make_quaternion (float radian_angle, Ogre :: Vector3 ax);

	boost :: shared_ptr <OgreOde :: Geometry> create_geometry (Reference <Items :: Item> item);
}

#endif	//	RADAKAN_MODEL_HPP
