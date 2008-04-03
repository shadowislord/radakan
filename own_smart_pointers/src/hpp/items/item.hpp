#ifndef RADAKAN_ITEM_HPP
#define RADAKAN_ITEM_HPP

#include "mesh_data.hpp"
#include "resident.hpp"

using namespace std;

namespace Radakan
{
	class Model;
	class Body;
	
	namespace Items
	{
		///	Item is the base class for all in-game items.
		class Item :
			public Resident <Item>
		{
			public :
				static string get_class_name ();
				
				virtual ~Item ();
				virtual bool is_initialized () const;
				
				virtual float get_total_mass () const;

				///	OgreOde doesn't need syncronization.
				#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
					void sync_model ();
				#endif

				void appear
					(Mathematics :: Vector_3D position, Mathematics :: Quaternion orientation);
				void disappear ();

				///	'get_body ()' may return an empty reference.
				Reference <Body> get_body () const;
				
				float get_volume () const;
				
				///	in kilograms
				///	'0 mass' means I'm a static item.
				const float mass;
				
				///	in cubic meters
				const Mathematics :: Vector_3D size;
				
				///	'false' means that you can pass through it.
				///	Default: 'true'
				const bool solid;
				
				const bool containable;
				
			protected :
				Item
				(
					float new_mass,
					Mathematics :: Vector_3D new_size,
					const Reference <Mesh_Data> new_mesh_data
				);
				
			private :
				Reference <Mesh_Data> mesh_data;
				
				Pointer <Model> model;
				Pointer <Body> body;
		};
	}
}

#endif	//	RADAKAN_ITEM_HPP
