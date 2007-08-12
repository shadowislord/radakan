#include "plugin_manager.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Engines;

//	static
const string Plugin_Manager ::
	get_class_name ()
{
	return "Plugin_Manager";
}

//  constructor
Plugin_Manager ::
	Plugin_Manager () :
	Object ("")
{
	//	Do nothing.

	assert (is_initialized ());
}

//  destructor
Plugin_Manager ::
	~Plugin_Manager ()
{
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Plugin_Manager ::
	is_initialized ()
	const
{
	assert (Singleton <Plugin_Manager> :: is_initialized ());
	
	return true;
}
