#ifndef RADAKAN_REFERENCE_BASE_HPP
#define RADAKAN_REFERENCE_BASE_HPP

#include <cassert>
#include <string>

#include <boost/utility.hpp>

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
			Reference_Base ();
			virtual ~Reference_Base ();
			virtual void destruct_from_parent () const = 0;

			static string get_class_name ();

			virtual string get_name () const;

			virtual void reset_pointee () = 0;
			
			virtual bool has_parent () const = 0;
			
		private :
			const string number;
			
			static long counter;
	};

	ostream & operator<< (ostream & out_stream, const Reference_Base & reference_base);
}

#endif	//	RADAKAN_REFERENCE_BASE_HPP
