#include <iostream>

using namespace std;

template <typename T> class Base
{
	public :
		template <typename U> void f ()
		{
			cout << "f ()" << endl;
		}
};

template <typename T> class Derived:
	Base <T>
{
	public :
		template <typename U> void g ()
		{
			//f <U> ();		 		//	gives "'f' was not declared in this scope"
			Base <T> :: f <U> ();	//	gives "expected primary-expression before '>' token"
		}
};

int main ()
{
	(new Derived <float> ()) -> g <int> ();

	return 0;
}
