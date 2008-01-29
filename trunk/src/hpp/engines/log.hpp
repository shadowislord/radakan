#ifndef RADAKAN_LOG_HPP
#define RADAKAN_LOG_HPP

#include "singleton.hpp"

using namespace std;

namespace Radakan
{
	namespace Engines
	{

		///	Log provides a logging system.
		class Log :
			public Singleton <Log>
		{
			public :
				static string get_class_name ();
				
				static void no_logs (const Reference_Base & logger);

				static ostream & error (const Reference_Base & logger);
				static ostream & log (const Reference_Base & logger);

				//	'show (...)' passes the message to my observers.
				static void show (string message);
				
				static void trace
				(
					const Reference_Base & logger,
					string class_name,
					string method = "",
					string argument_1 = "",
					string argument_2 = "",
					string argument_3 = "",
					string argument_4 = "",
					string argument_5 = "",
					string argument_6 = ""
				);

				Log ();
				virtual ~Log ();
				virtual bool is_initialized () const;
		};
	}
}

#endif	//	RADAKAN_LOG_HPP
