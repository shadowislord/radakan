#include "world.hpp"
#include "shr.hpp"

using namespace std;

vector <Object *> objects;

int
	main ()
{
	World * world = new World ("tsl", 10, 5);

	cout << "World initialised." << endl;

	delete world;
	
	cout << "Objects left: " << endl;
	for (unsigned int i = 0; i < objects.size (); i++)
	{
		if (objects.at (i) != NULL)
		{
			cout << i << ": " << * objects.at (i) << endl;
		}
	}
	cout << endl;
	
	return 0;
}
