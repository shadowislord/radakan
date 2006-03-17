#include "tinyxml/tinyxml.h"
#include "world.hpp"
#include "shr.hpp"

using namespace std;

vector <Object *> objects;

int
	main ()
{
	TiXmlDocument doc;
	doc.LoadFile ("data/test_world.xml");
	TiXmlElement * xml;

	xml = doc.RootElement();
	assert (xml != NULL);
	assert (!xml->NoChildren ());
	
	World * tsl = new World ("tsl", 10, 5);
	Obstacle * obstacle = new Obstacle ("abc");
	
	tsl->get_tile (7, 4)->set_obstacle (obstacle);

	delete tsl;
	
	bool objects_left = false;
	for (unsigned int i = 0; i < objects.size (); i++)
	{
		if (objects.at (i) != NULL)
		{
			if (! objects_left)
			{
				cout << "Objects left: " << endl;
				objects_left = true;
			}
			cout << i << ": " << * objects.at (i) << endl;
		}
	}
	if (! objects_left)
	{
		cout << "tslrpg finished successfully. " << endl;
		return 0;
	}
	cerr << "ERROR: tslrpg finished with orphaned objects. " << endl;
	return 1;
}
