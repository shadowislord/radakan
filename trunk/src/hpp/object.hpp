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
			Object (string new_name, string automatic_destruction_prevention_key = "");
			virtual ~Object ();
			virtual bool is_initialized () const;
			
			///	Call 'prepare_for_destruction ()' at the beginning of the
			///	destructor of each non-abstract subclass.
			///	Don't worry about calling it twice.
			///	Each reference to me will be un-set.
			void prepare_for_destruction ();

			void register_reference
				(const Reference_Base & reference, bool weak = false) const;

			void unregister_reference (const Reference_Base & reference) const;

			#ifdef RADAKAN_DEBUG	
				void list_references () const;
			#endif

			///	'key' may not be the empty string.
			void add_automatic_destruction_prevention (string key);
			
			///	'key' may not be the empty string.
			void remove_automatic_destruction_prevention (string key);
			
			const string name;
			
		private :
			bool has_dependency () const;
			bool does_depend (const Reference_Base & candidate) const;

			boost :: scoped_ptr <set <string> > automatic_destruction_preventions;

			#ifdef RADAKAN_DEBUG	
				bool is_tracked;
			#endif

			///	I store my dependencies as const to reduce the number of casts,
			///	but they are 'const_cast'-ed, at my destruction.
			mutable boost :: scoped_ptr <set <const Reference_Base *> > strong_dependencies;

			///	I store my dependencies as const to reduce the number of casts,
			///	but they are 'const_cast'-ed, at my destruction.
			mutable boost :: scoped_ptr <set <const Reference_Base *> > weak_dependencies;

		public :
			Reference <Object> me;
	};
}

#endif	//	RADAKAN_OBJECT_HPP
