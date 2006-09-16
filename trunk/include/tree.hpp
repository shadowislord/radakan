#ifndef TREE_HPP
#define TREE_HPP

#include "object.hpp"

using namespace std;

class Tree:
	public virtual Object
{
	public:
		Tree (string new_name);
		virtual ~Tree ();
		virtual bool is_initialized () const;

		virtual bool add (Object * sub_tree);				//	true iff succes
		virtual bool contains (Object * sub_tree, bool recursive) const;
		virtual bool move_to (Object * sub_tree, Object * other_tree);
															//	true iff succes
		template <typename T> T * get_child () const;

		set <Object *> children;
};

#include "tree.ipp"

#endif
