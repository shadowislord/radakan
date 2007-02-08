#ifndef TSL_ITEM_HPP
#define TSL_ITEM_HPP

#include "representation.hpp"

using namespace std;

namespace tsl
{
	class Item :
		public virtual Object
	{
		public :
			virtual ~Item ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			virtual float get_total_weight () const;

			static Item & create
			(
				string new_name,
				string new_mesh_name,
				float new_volume,
				float new_weight,
				bool new_mobile = true,
				bool new_solid = true,
				bool new_visible = true
			);

			void add_representation (Ogre :: SceneNode & node, OgreOde :: World * world);
			bool has_representation () const;
			void remove_representation ();
			Representation & get_representation () const;

			const string mesh_name;
			
			///	in litres
			const float volume;
			
			///	in kilograms
			const float weight;
			
			///	false means that the object is static
			const bool mobile;
			
			///	false means that you can pass through it
			const bool solid;
			
			const bool visible;

		protected :
			Item
			(
				string new_name,
				string new_mesh_name,
				float new_volume,
				float new_weight,
				bool new_mobile,
				bool new_solid,
				bool new_visible
			);

		private :
			//	Copies are not allowed.
			Item (const Item & item);
			
			///	can be NULL
			Representation * representation;
	};
}

#endif	//	TSL_ITEM_HPP
