/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#ifndef _OGREODEMAINTAINEDLIST_H_
#define _OGREODEMAINTAINEDLIST_H_

#include "OgreOdePreReqs.h"

/*!
 * \brief
 * Write brief comment for OgreOde here.
 * 
 * Write detailed description for OgreOde here.
 * 
 * \remarks
 * Write remarks for OgreOde here.
 * 
 * \see
 * Separate items with the '|' character.
 */
namespace OgreOde
{

	/** Convenience class to make it easy to step through all MaintainedItem in a MaintainedList.
	*/
	template <class T>
	class _OgreOdeExport MaintainedItemIterator
	{

	public:
		typedef std::deque<T*> MaintainedItemList;
		typedef std::map<unsigned long,T*> MaintainedItemMap;


	protected:
		typename MaintainedItemList::iterator mPos;
		typename MaintainedItemList::iterator mStart;
		typename MaintainedItemList::iterator mEnd;


	public:
		/// constructor, only available for MaintainedList::getIterator
		MaintainedItemIterator(typename MaintainedItemList::iterator start, typename MaintainedItemList::iterator end)
		{
			mStart = mPos = start;
			mEnd = end;
		}
		// Returns true when at the end of the MaintainedItem list
		inline bool end(void) const 
		{
			return (mPos == mEnd);
		}
		/** Returns a pointer to the next MaintainedItem, and moves the iterator on by 1 element. */
		inline T* getNext(void)
		{
			return static_cast<T*>(*mPos++);
		}
	};

	template <class T>
	class _OgreOdeExport MaintainedList
	{
	public:
		typedef std::deque<T*>				MaintainedItemList;
		typedef std::map<unsigned long,T*>	MaintainedItemMap;

	public:
		MaintainedList(){};
		~MaintainedList(){};

		void registerItem(T* ptr)	
		{
			assert(ptr->getID());
			_map[ptr->getID()] = ptr;
			_list.push_back (ptr);
		}

		T* findItem(unsigned long id)
		{
			assert (_map.find(id) != _map.end());
			return _map[id];
		}

		void unregisterItem(unsigned long id)
		{
			assert (_map.find(id) != _map.end());
			T* m = _map[id];
			_map.erase(id);

			typename MaintainedItemList::iterator i = std::find(_list.begin (), 
				_list.end (), 
				m);
			if(i != _list.end())
			{
				assert(*i == m);
				_list.erase(i);
			}
		}

		void setDebug(const bool debug)
		{
			typename MaintainedItemList::iterator i = _list.begin();
			typename MaintainedItemList::iterator end = _list.end();
			for(;i != end;++i)
			{
				(*i)->setDebug(debug);
			}
        }

        void setDebugContact(const bool debug)
        {
            typename MaintainedItemList::iterator i = _list.begin();
            typename MaintainedItemList::iterator end = _list.end();
            for(;i != end;++i)
            {
                (*i)->setDebugContact(debug);
            }
        }

        void updateDebugContact()
        {
            std::for_each(_list.begin (), 
                _list.end (),  
                std::mem_fun(&T::updateDebugContact));
        }



		void synchronise()
		{
			std::for_each(_list.begin (), 
				_list.end (),  
				std::mem_fun(&T::synchronise));
		}

		void updateDrawState()
		{
			std::for_each(_list.begin (), 
				_list.end (),  
				std::mem_fun(&T::updateDrawState));
		}
		void updatePreviousState()
		{
			std::for_each(_list.begin (), 
				_list.end (),  
				std::mem_fun(&T::updatePreviousState));
		}

		void updateCurrentState()
		{
			std::for_each(_list.begin (), 
				_list.end (),  
				std::mem_fun(&T::updateCurrentState));
		}

		void interpolateDrawState(const Ogre::Real alpha)
		{
			typename MaintainedItemList::iterator i = _list.begin();
			typename MaintainedItemList::iterator end = _list.end();
			for(;i != end;++i)
			{
				(*i)->interpolateDrawState(alpha);
			}
		}

		void notify(Body* body)
		{
			typename MaintainedItemList::iterator i = _list.begin();
			typename MaintainedItemList::iterator end = _list.end();
			for(;i != end;++i)
			{
				(*i)->notify(body);
			}
		}

		MaintainedItemIterator<T> getIterator(void)
		{
			return MaintainedItemIterator<T>(_list.begin(), _list.end());
		}

	protected:

		MaintainedItemMap  _map;
		MaintainedItemList _list;
	};

}

#endif
