#ifndef RADAKAN_OBJECT_HPP
#define RADAKAN_OBJECT_HPP

#include "reference.hpp"

using namespace std;

namespace Radakan
{

	///	I'm the abstract base class for all Radakan classes.
	///	I can't be copied, consider a reference of me instead.
	class Object :
		public boost :: noncopyable
	{
		public :
			static string get_class_name ();

			#ifdef RADAKAN_WINDOWS
				///	Some Windows compilers give an error otherwise.
				Object ();
			#endif
			Object (string new_name, bool new_prevent_automatic_destruction = false);
			virtual ~Object ();
			virtual bool is_initialized () const;
			
			///	Call 'prepare_for_destruction ()' at the beginning of the
			///	destructor of each non-abstract subclass.
			///	Don't worry about calling it twice.
			///	'destructing' is set to 'true'.
			///	Each reference to me will be un-set.
			void prepare_for_destruction ();

			void register_reference (const Reference_Base & reference) const;

			void unregister_reference (const Reference_Base & reference) const;

			bool is_destructing () const;
			
			const string name;
			
			bool prevent_automatic_destruction;

		private :
			bool has_dependency () const;
			bool does_depend (const Reference_Base & candidate) const;

			#ifdef RADAKAN_DEBUG	
				bool is_tracked;
			#endif

			///	I store my dependencies as const to reduce the number of casts,
			///	but they are 'const_cast'-ed, at my destruction.
			mutable boost :: scoped_ptr <set <const Reference_Base *> > dependencies;

			///	'status' can be 'constructing', 'running' or 'destructing'.
			string status;

		public :
			Reference <Object> me;
	};
}

#endif	//	RADAKAN_OBJECT_HPP
