//  The universal base class

#ifndef OBJECT_H
#define OBJECT_H

#define PAUSE {print()<<"pause";getchar();}
#define TAB "    "

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Object: public string
{
	public:
		//	Constructor
		Object (string new_name);
		//	Destructor
		virtual ~Object ();
		template <typename T> T * copy () const;
		template <typename T> bool is_type () const;
		template <typename T> T * to_type () const;
		virtual bool is_initialized () const;
		virtual ostream& print () const;
		virtual ostream& debug () const;
		virtual ostream& error () const;
};

extern vector <Object *> objects;

string to_string (int value);
int to_int (string value);

#include "object.inc"

#endif

