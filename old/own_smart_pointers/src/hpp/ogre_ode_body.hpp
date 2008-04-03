#ifndef RADAKAN_OGREODE_BODY_HPP
#define RADAKAN_OGREODE_BODY_HPP
	#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE

	#include "body.hpp"

	using namespace std;

	namespace OgreOde
	{
		class Geometry;
		class Space;
	}
	namespace Radakan
	{
		
		class OgreOde_Body :
			public Body
		{
			public :
				OgreOde_Body
					(Reference <Items :: Item> new_item, Mathematics :: Vector_3D position);
				virtual ~OgreOde_Body ();
				virtual bool is_initialized () const;
				
				static string get_class_name ();
				
				virtual Mathematics :: Vector_3D get_position () const = 0;
				virtual Mathematics :: Quaternion get_orientation () const = 0;
				
				virtual void apply_force (Mathematics :: Vector_3D force);
				virtual void apply_torque (Mathematics :: Vector_3D torque);
				
				///	Reset the position and orientation.
				virtual void reset ();
		
			private :
				static boost :: shared_ptr <OgreOde :: Geometry> create_geometry
					(Reference <Items :: Item> item);
					
				boost :: shared_ptr <OgreOde :: Geometry> geometry;
				boost :: shared_ptr <OgreOde :: Body> body;
		};
	}

	#endif
#endif	//	RADAKAN_OGREODE_BODY_HPP
