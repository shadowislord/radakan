#ifndef RADAKAN_REFERENCE_BASE_HPP
#define RADAKAN_REFERENCE_BASE_HPP

#include "base.hpp"

using namespace std;

namespace Radakan
{
	string bool_to_string (const bool & value);
	string to_string (const float & value);
	float to_float (const string & value);

	class Reference_Base :
		public boost :: noncopyable
	{
		public :
			static string get_class_name ();

			Reference_Base ();
			virtual ~Reference_Base ();
			virtual void destruct () const = 0;
			
			virtual string get_name () const;
			
		private :
			const string number;
			
			static long counter;
	};

	ostream & operator<< (ostream & out_stream, const Reference_Base & reference_base);
}

#endif	//	RADAKAN_REFERENCE_BASE_HPP
