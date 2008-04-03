#ifndef SCRIPT_H
#define SCRIPT_H

/* ----------------------------------------------------------------------------
   MIGHTY::Script

   Copyright (C) 2006 Neil Lalonde

   This program is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation; either version 2 of the License, or (at your option) any later
   version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with
   this program; if not, write to the Free Software Foundation, Inc., 59 Temple
   Place, Suite 330, Boston, MA 02111-1307 USA
----------------------------------------------------------------------------- */


#include "stdlib.h"
#include "stdio.h"
#include <iostream>
#include <sstream>
#include <string>
#include "exception.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


namespace MIGHTY
{
	//------------------------------------------------------------------------
	// ScriptException
	// :--> MIGHTY::Script objects throw this type of exception.
	//------------------------------------------------------------------------
	class ScriptException : public Exception
	{
	public:
		ScriptException( const char * aszMsg )
			: Exception( aszMsg ) {};
	};

	//------------------------------------------------------------------------
	// Script
	// 
	// This object encapsulates a Lua script and does some Lua error 
	// handling.  Lua errors will be thrown as ScriptExceptions.
	// This class should be extended by other classes in order to add
	// new C++ functions that the scripts can access.  This class provides
	// no C functions for Lua scripts.
	//------------------------------------------------------------------------
	class Script
	{
	public:
		Script()
		{
			mbLuaState = false;
			mbLoaded   = false;
		};

		~Script()
		{
			cleanup();
		};

		lua_State *		mlState;

		//----------------------------------------------------------
		// cleanup
		// :--> Call this function to unload the script(s) and 
		//		reset the state of this Script object.
		//		This must be done if you want to re-use the Script
		//		object with a fresh state.
		//----------------------------------------------------------
		void cleanup( void )
		{
			if( mbLuaState )
			{
				lua_close( mlState );
				mbLuaState = false;
				mbLoaded   = false;
			}
		}

		//----------------------------------------------------------
		// loadScript
		// :--> Load a script (a Lua chunk), but do not run it yet.
		//		ie, The script virtual machine will not be aware
		//		of any new functions, variables, etc in the script
		//		after this function call is made.  Call runScript
		//		to do that.
		//		This adds the contents of the script to the Script
		//		state.  runScript executes the newly loaded script
		//		and incorporates it into the existing state.
		//----------------------------------------------------------
		virtual void loadScript( const std::string& aFilename );

		//----------------------------------------------------------
		// runScript
		// :--> Run a script (Lua chunk) that was loaded with
		//		loadScript.
		//----------------------------------------------------------
		virtual void runScript( void );

		//-----------------------------------------------------------
		// isLoaded
		// :--> Return true if a script is loaded.
		//-----------------------------------------------------------
		bool isLoaded( void )
		{
			return( mbLoaded );
		}

		//-----------------------------------------------------------
		// callFunction
		// :--> Code from "Programming in Lua"
		//		http://www.lua.org/pil/25.3.html
		//-----------------------------------------------------------
		void callFunction(const char *func, const char *sig, ...);

		//-----------------------------------------------------------
		// addStringGlobal
		// :--> Add a global string variable to the script's 
		//		environment.  You can only do this after a script
		//		has been loaded and run.
		//-----------------------------------------------------------
		void addStringGlobal( const std::string& aName,
							  const std::string& aValue );

		//-----------------------------------------------------------
		// addNumberGlobal
		// :--> Add a global number (float) variable to the script's 
		//		environment.  You can only do this after a script
		//		has been loaded and run.
		//-----------------------------------------------------------
		void addNumberGlobal( const std::string& aName,
							  double aValue );

		//-----------------------------------------------------------
		// getStringGlobal
		// :--> Get a string global from the script's environment.
		//-----------------------------------------------------------
		const std::string& getStringGlobal( const std::string& aName );


		//-----------------------------------------------------------
		// getNumberGlobal
		// :--> Get a number global from the script's environment.
		//-----------------------------------------------------------
		double getNumberGlobal( const std::string& aName );

		//-----------------------------------------------------------
		// getBoolGlobal
		// :--> Get a boolean global from the script's environment.
		//-----------------------------------------------------------
		bool getBoolGlobal( const std::string& aName );

		//-----------------------------------------------------------
		// addStringGlobalToTable
		// :--> Add a global string variable to the script's 
		//		environment in an existing table.
		//		You can only do this after a script has been loaded
		//		and run.
		//-----------------------------------------------------------
		void addStringGlobalToTable( const std::string& aTable,
									 const std::string& aName,
									 const std::string& aValue );

		//-----------------------------------------------------------
		// addNumberGlobalToTable
		// :--> Add a global number variable to the script's 
		//		environment in an existing table.
		//		You can only do this after a script has been loaded
		//		and run.
		//-----------------------------------------------------------
		void addNumberGlobalToTable( const std::string& aTable,
									 const std::string& aName,
									 double aValue );

		//-----------------------------------------------------------
		// getStringGlobalFromTable
		// :--> Get a global string variable from the script's 
		//		environment in an existing table.
		//		You can only do this after a script has been loaded
		//		and run.
		//-----------------------------------------------------------
		const std::string& getStringGlobalFromTable(
									 const std::string& aTable,
									 const std::string& aName );

		//-----------------------------------------------------------
		// getStringGlobalFromTable
		// :--> Get a global string variable from the script's 
		//		environment in an existing table.
		//		You can only do this after a script has been loaded
		//		and run.
		//-----------------------------------------------------------
		const std::string& getStringGlobalFromTable(
									 const std::string& aTable,
									 const int			aIndex );

		//-----------------------------------------------------------
		// addNumberGlobalToTable
		// :--> Get a global number variable from the script's 
		//		environment in an existing table.
		//		You can only do this after a script has been loaded
		//		and run.
		//-----------------------------------------------------------
		double getNumberGlobalFromTable( const std::string& aTable,
									 const std::string& aName );

		//-----------------------------------------------------------
		// globalExists
		// :--> Return true if a global variable exists with the
		//		given name.
		//-----------------------------------------------------------
		bool globalExists( const std::string& aName );

		//-----------------------------------------------------------
		// globalExistsInTable
		// :--> Return true if a global variable exists with the
		//		given name in a given table.
		//-----------------------------------------------------------
		bool globalExistsInTable( const std::string& aTable,
								  const std::string& aName );
		bool globalExistsInTable( const std::string& aTable,
								  const int aIndex );

		//-----------------------------------------------------------
		// stackDump
		// :-->	Print out the contents of the Lua stack into a 
		//		string.
		//-----------------------------------------------------------
		const std::string& stackDump( void );

		std::string		sScriptName;

//-------------------------------------------------------------------
// Static versions of the above functions.
// The static versions have a lua_state* argument so that they can
// be used whenever a lua_state is given that is not part of a known
// MIGHTY::Script object.  This is especially useful in any 
// C functions provided to Lua scripts.
//-------------------------------------------------------------------
		static void addStringGlobal(lua_State *L,  const std::string& aName,
							  const std::string& aValue );
		static void addNumberGlobal(lua_State *L,  const std::string& aName,
							  double aValue );
		static const std::string& getStringGlobal(lua_State *L,  const std::string& aName );
		static double getNumberGlobal(lua_State *L,  const std::string& aName );
		static bool getBoolGlobal( lua_State *L, const std::string& aName );
		static void addStringGlobalToTable(lua_State *L,  const std::string& aTable,
									 const std::string& aName,
									 const std::string& aValue );
		static void addNumberGlobalToTable(lua_State *L,  const std::string& aTable,
									 const std::string& aName,
									 double aValue );
		static const std::string& getStringGlobalFromTable(lua_State *L,
									 const std::string& aTable,
									 const std::string& aName );
		static const std::string& getStringGlobalFromTable(lua_State *L,
									 const std::string& aTable,
									 const int			aIndex );
		static double getNumberGlobalFromTable(lua_State *L,  const std::string& aTable,
									 const std::string& aName );
		static bool globalExists(lua_State *L,  const std::string& aName );
		static bool globalExistsInTable(lua_State *L,  const std::string& aTable,
								  const std::string& aName );
		static bool globalExistsInTable(lua_State *L,  const std::string& aTable,
								  const int aIndex );
		static const std::string& stackDump( lua_State *L );

	protected:
		//----------------------------------------------------------
		// setup
		// :--> Sets up the Lua stuff so that we can load and 
		//		run scripts in this object with a fresh state.
		//----------------------------------------------------------
		virtual void setup( void );

		//----------------------------------------------------------
		// catchLuaError
		// :--> Rather than allow Lua to call exit() when it hits
		//		an error in a script, we register this static
		//		method so that we can throw our own exception 
		//		instead and handle the problem our own way.
		//----------------------------------------------------------
		static int catchLuaError( lua_State *L )
		{
			std::cout << "Lua error caught in Script!" << std::endl;

			throw( MIGHTY::Exception( "Lua script badness occurred!" ) );

			// Throw is a long jump, so this function never returns.
			// If it did, Lua would call exit() and the whole 
			// application would terminate badly.
			return( 0 );
		}

		//----------------------------------------------------------
		// registerCFunctions
		// :--> This function is called to register the C/C++
		//		routines that the Lua script will access.
		//		Derived classes should implement this function.
		//----------------------------------------------------------
		virtual void registerCFunctions( void )
		{
			// EXAMPLES:

			// lua_register( mlState, "callIt", callIt);
			// lua_register( mlState, "callThis", CallMe::callThis );
		}

		//----------------------------------------------------------
		// getLuaErrorString
		// :--> Converts an error integer from Lua into a readable
		//		string.
		//----------------------------------------------------------
		const std::string& getLuaErrorString( int aiErr )
		{
			static std::string aErrString;

			switch( aiErr )
			{
			case LUA_ERRRUN:
				aErrString = "LUA_ERRRUN";
				break;
			case LUA_ERRFILE:
				aErrString = "LUA_ERRFILE";
				break;
			case LUA_ERRSYNTAX:
				aErrString = "LUA_ERRSYNTAX";
				break;
			case LUA_ERRMEM:
				aErrString = "LUA_ERRMEM";
				break;
			case LUA_ERRERR:
				aErrString = "LUA_ERRERR";
				break;
			default:
				aErrString = "Unknown";
				break;
			}

			return( aErrString );
		}

		//----------------------------------------------------------
		// Data members
		//----------------------------------------------------------
		bool			mbLuaState; // true = open, false = closed.
		bool			mbLoaded;   // true = script loaded

		static std::stringstream mssOut;

	};

	
};



#endif // SCRIPT_H
