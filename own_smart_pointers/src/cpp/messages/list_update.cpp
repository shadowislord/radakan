#include "engines/log.hpp"
#include "messages/list_update.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Messages;

//	static
string List_Update ::
	get_class_name ()
{
	return "Messages :: List_Update";
}

//	static
Reference <List_Update> List_Update ::
	create (string new_item, string new_list_name)
{
	boost :: shared_ptr <set <string> > items (new set <string>);
	items -> insert (new_item);
	
	return Reference <List_Update>
		(new List_Update (new_item, items, new_list_name, false));
}

//  constructor
List_Update ::
	List_Update
	(
		string new_name,
		boost :: shared_ptr <set <string> > new_items,
		string new_list_name,
		bool new_reset
	) :
	Object (new_name),
	items (new_items),
	list_name (new_list_name),
	reset (new_reset)
{
	Engines :: Log :: trace (me, List_Update :: get_class_name (), new_name, "@new_items@",
		new_list_name, bool_to_string (new_reset));
	assert (Object :: is_initialized ());
	
	//	Do nothing.
	
	assert (is_initialized ());
}

//  destructor
List_Update ::
	~List_Update ()
{
	Engines :: Log :: trace (me, List_Update :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool List_Update ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}
