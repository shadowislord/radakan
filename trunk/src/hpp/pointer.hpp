#ifndef RADAKAN_POINTER_HPP
#define RADAKAN_POINTER_HPP

#include "reference.hpp"

using namespace std;

namespace Radakan
{
	template <class T> class Set;

	template <class T> class Pointer :
		public Reference <T>
	{
		public :
			static string get_class_name ();
			
			Pointer & operator= (const Reference <T> & other);
			template <class U> Pointer & operator= (const Reference <U> & other);

			Pointer (T * new_pointee = NULL);
			Pointer (const Pointer <T> & other);
			Pointer (const Reference <T> & other);
			template <class U> Pointer (const Reference <U> & other);
			virtual ~Pointer ();

			void reset_pointee (T * new_pointee = NULL);
	};
}

#endif	// RADAKAN_POINTER_HPP
