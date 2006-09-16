#include "hit_event.hpp"

using namespace std;

//  constructor
Hit_Event ::
	Hit_Event (Character * new_attacker, Character * new_defender):
	Object (((new_attacker == NULL) ? string ("[ERROR: new_attacker is NULL]") : * new_attacker) + " hits " + ((new_defender == NULL) ? string ("[ERROR: defender is NULL]") : * new_defender)),
	Event (((new_attacker == NULL) ? string ("[ERROR: new_attacker is NULL]") : * new_attacker) + " hits " + ((new_defender == NULL) ? string ("[ERROR: defender is NULL]") : * new_defender))
{
	assert (Event :: is_initialized ());
	assert (new_attacker != NULL);
	assert (new_defender != NULL);
	assert (new_attacker->is_initialized ());
	assert (new_defender->is_initialized ());

	attacker = new_attacker;
	defender = new_defender;

	assert (is_initialized ());
}

//  destructor
Hit_Event ::
	~Hit_Event ()
{
	assert (Object :: is_initialized (* this + "->~Hit_Event ()"));
}

//	virtual
bool Hit_Event ::
	is_initialized ()
	const
{
	return Event :: is_initialized () && (attacker != NULL) &&  (defender != NULL) && attacker->is_initialized () && defender->is_initialized ();

}
