#ifndef TREE_IPP
#define TREE_IPP

template <typename T> T * Tree ::
	get_child ()
	const
{
	for (set <Object *> :: const_iterator i = children.begin ();
													i != children.end (); i ++)
	{
		if ((* i) -> is_type <T> ())
		{
			return (*i) -> to_type <T> ();
		}
	}
	return NULL;
}

#endif	//	TREE_IPP
