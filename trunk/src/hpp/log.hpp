#ifndef TSL_LOG_HPP
#define TSL_LOG_HPP

#include "singleton.hpp"
#include "observable.hpp"

using namespace std;

namespace TSL
{
	class Log :
		public Singleton <Log>,
		public Observable <Log>
	{
		public :
			Log ();
			virtual ~Log ();
			virtual bool is_initialized () const;
			
			static const string class_name;

			ostream & error (const Object & logger) const;
			ostream & log (const Object & logger, bool condition = true) const;
			void show (string message, bool condition = true) const;

		private :
			/// for messages that should be ignored
			ostream the_abyss;
	};
}

#endif	//	TSL_LOG_HPP
