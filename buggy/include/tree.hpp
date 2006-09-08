#ifndef TREE_HPP
#define TREE_HPP

#include "object.hpp"

using namespace std;

template <typename T> class Tree:
	public Object
{
	public:
		Tree (string new_name);
		Tree (T * new_t);
		virtual ~Tree ();
		virtual bool is_initialized () const;
		virtual bool is_initialized (string debug_message) const;

		//	The methods below are not recursive.

		virtual Tree <T> * add (T * t);		//	returns t's subtree, iff succes
		virtual bool add (Tree <T> * tree);					//	true iff succes
		virtual bool contains (T * t) const;
		virtual bool contains (Tree <T> * tree) const;

		///	Move item to new_tree.
		virtual bool move_to (T * t, Tree <T> * other_tree);
															//	true iff succes
		virtual bool move_to (Tree <T> * sub_tree, Tree <T> * other_tree);
															//	true iff succes

		T * data;
		set <Tree <T> *> children;
};

#include "tree.ipp"

#endif
