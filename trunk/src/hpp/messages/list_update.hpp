#ifndef RADAKAN_MESSAGES_LIST_UPDATE_HPP
#define RADAKAN_MESSAGES_LIST_UPDATE_HPP

#include "object.hpp"

using namespace std;

namespace Radakan
{
	namespace Messages
	{

		///	TODO Add documentation.
		class List_Update :
			virtual public Object
		{
			public :
				static string get_class_name ();

				static Reference <List_Update> create
					(string new_item, string new_list_name);

				List_Update
				(
					string new_name,
					boost :: shared_ptr <set <string> > new_items,
					string new_list_name,
					bool new_reset
				);
				virtual ~List_Update ();
				virtual bool is_initialized () const;

				const boost :: shared_ptr <set <string> > items;
				const string list_name;
				const bool reset;
		};
	}
}

#endif	// RADAKAN_MESSAGES_LIST_UPDATE_HPP
