#ifndef TSL_ITEM_HPP
#define TSL_ITEM_HPP

#include "body.hpp"
#include "observable.hpp"

using namespace std;

namespace tsl
{
	class Item :
		public Observable <Body>
	{
		public :
			virtual ~Item ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			virtual float get_total_mass () const;

			void set_body (OgreOde :: World & world);
			bool has_body () const;
			void remove_body ();
			Body & get_body () const;

			///	in litres
			const float volume;
			
			///	in kilograms
			const float mass;
			
			///	false means that the object is static
			const bool mobile;
			
			///	false means that you can pass through it
			const bool solid;
			
			const bool visible;

			Ogre :: Entity & entity;

			static Ogre :: SceneManager * scene_manager;

		protected :
			Item
			(
				string new_mesh_name,
				float new_volume,
				float new_mass,
				bool new_mobile,
				bool new_solid,
				bool new_visible
			);

		private :
			//	Copies are not allowed.
			Item (const Item & item);

			///	can be NULL
			Body * body;
	};
}

#endif	//	TSL_ITEM_HPP
