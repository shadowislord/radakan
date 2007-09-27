#ifndef RADAKAN_REFERENCE_BASE_HPP
#define RADAKAN_REFERENCE_BASE_HPP

#include <cassert>
#include <string>

#include <boost/utility.hpp>

using namespace std;

namespace Radakan
{
	class Reference_Base :
		public boost :: noncopyable
	{
		public :
			Reference_Base ();
			virtual ~Reference_Base ();

			static string get_class_name ();

			virtual string get_name () const;
			
		private :
			const int number;
			
			static int counter;
	};
}

#endif	//	RADAKAN_REFERENCE_BASE_HPP
