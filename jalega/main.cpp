#include "world.hpp"
#include "shr.hpp"

using namespace std;

vector <Object *> objects;

int
	main ()
{

	
	World * tsl = new World ("test_world");
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
