#include "engines/log.hpp"
#include "mesh_data.hpp"

using namespace std;
using namespace Radakan;

//	static
string Mesh_Data ::
	get_class_name ()
{
	return "Mesh_Data";
}

//  constructor
Mesh_Data ::
	Mesh_Data (string new_file_name) :
	Object ("mesh based on " + new_file_name),
	file_name (new_file_name),
	default_orientation (Mathematics :: Quaternion :: identity),
	material_file_name (""),
	scale (1)
{
	Engines :: Log :: trace (me, Mesh_Data :: get_class_name (), "", new_file_name);

	assert (Mesh_Data :: is_initialized ());
}

//  destructor
Mesh_Data ::
	~Mesh_Data ()
{
	Engines :: Log :: trace (me, Mesh_Data :: get_class_name (), "~");
	assert (Mesh_Data :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Mesh_Data ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());
	assert (! file_name . empty ());
	assert (0 < scale);

	return true;
}
