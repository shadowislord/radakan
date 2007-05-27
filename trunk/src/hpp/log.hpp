#ifndef TSL_LOG_HPP
#define TSL_LOG_HPP

#include "singleton.hpp"
#include "observable.hpp"

using namespace std;

namespace TSL
{
	namespace Engines
	{

		///	Log provides a logging system.
		class Log :
			public Singleton <Log>,
			public Observable <Log>
		{
			public :
				Log ();
				virtual ~Log ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				static ostream & error (const Object & logger);
				static ostream & log (const Object & logger);
				static void show (string message);
				template <class T> static void trace
				(
					const Object & logger,
					string method = "",
					string argument_1 = "",
					string argument_2 = "",
					string argument_3 = "",
					string argument_4 = "",
					string argument_5 = "",
					string argument_6 = ""
				);
		};
	}
}

#endif	//	TSL_LOG_HPP
