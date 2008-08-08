#ifndef RADAKAN_REFERENCE_BASE_HPP
#define RADAKAN_REFERENCE_BASE_HPP

#include "base.hpp"

using namespace std;

namespace Radakan
{
	class Reference_Base :
		public boost :: noncopyable
	{
		public :
			static string get_class_name ();

			Reference_Base ();
			virtual ~Reference_Base ();
			virtual void destruct () const = 0;
			
			virtual string get_name (bool suppress_debug_info = false) const;
			
		private :
			const string number;
			
			static unsigned int counter;
	};

	ostream & operator<< (ostream & out_stream, const Reference_Base & reference_base);
}

#endif	//	RADAKAN_REFERENCE_BASE_HPP
