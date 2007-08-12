#include "chat_state.hpp"
#include "plugins/conversation_plugin.hpp"
#include "gui_engine.hpp"
#include "log.hpp"
#include "settings.hpp"

#include <CEGUIExceptions.h>
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxTextItem.h>

using namespace std;
using namespace TSL;
using namespace TSL :: Plugins;

//	static
const string Conversation_Plugin ::
	get_class_name ()
{
	return "Conversation_Plugin";
}

//  constructor
Conversation_Plugin ::
	Conversation_Plugin () :
	Object ("conversation plugin"),
	behavior (Engines :: Settings :: get () . tsl_path + "/data/behavior.xml")
{
	bool check = behavior . LoadFile ();
	if ((! check) || behavior . Error ())
	{
		Engines :: Log :: error (me) << behavior . ErrorDesc () << endl;
		abort ();
	}

	assert (is_initialized ());
}

//  destructor
Conversation_Plugin ::
	~Conversation_Plugin ()
{
	Engines :: Log :: trace (me, Conversation_Plugin :: get_class_name (), "~");
	assert (is_initialized ());
	
	prepare_for_destruction ();
}

//	virtual
bool Conversation_Plugin ::
	is_initialized ()
	const
{
//	assert (Singleton <Conversation_Plugin> :: is_initialized ());
	assert (Observer <Strategies :: Play_State> :: is_initialized ());
	
	return true;
}

//	virtual
void Conversation_Plugin ::
	call (const Object & message)
{
	assert (is_initialized ());

	TiXmlElement * root = behavior . RootElement ();
	assert (root != NULL);
	TiXmlElement * peace_state = root -> FirstChildElement ();
	assert (peace_state != NULL);
	TiXmlElement * options = peace_state -> FirstChildElement ();
	assert (options != NULL);

	for (TiXmlElement * option = options -> FirstChildElement ("option");
				option != NULL; option = option -> NextSiblingElement ("option"))
	{
		string message = option -> Attribute ("say");
		assert (! message . empty ());
		
		CEGUI :: ListboxTextItem * item = new CEGUI :: ListboxTextItem (message);
		
		CEGUI :: Listbox & chat_window = * Strategies :: Play_State :: get () . chat_window;
		
		chat_window . addItem (item);
		chat_window . ensureItemIsVisible (chat_window . getItemCount ());
	}
}
