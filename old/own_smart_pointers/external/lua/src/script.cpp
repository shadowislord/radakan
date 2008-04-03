#include "script.h"
#include <stdarg.h>

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

std::stringstream MIGHTY::Script::mssOut;

//----------------------------------------------------------
// loadScript
// :--> Load a script, but do not run it yet.
//		ie, The script virtual machine will not be aware
//		of any functions, variables, etc in the script
//		after this function call is made.  Call runScript
//		to do that.
//----------------------------------------------------------
void MIGHTY::Script::loadScript( const std::string& aFilename )
{
	// If we have not got a Lua state
	if( !mbLuaState /*|| mbLoaded*/ )
	{
		setup();
		sScriptName = aFilename;
	}

	// Now load the script
	int s = luaL_loadfile( mlState, aFilename.c_str() );

	if( s == 0 )
	{
		mbLoaded = true;
	}
	else
	{
		std::string errorMsg;

		errorMsg = lua_tostring( mlState, -1 );

		mssOut.str( "" );
		mssOut << "Failed to load script "
			<< aFilename << "\nwith the following Lua error: "
			<< getLuaErrorString( s ) << ".\n"
			<< "    Error msg is: " << errorMsg;

		throw( ScriptException( mssOut.str().c_str() ) );
	}
}

//----------------------------------------------------------
// runScript
// :--> Run a script that was loaded with loadScript.
//----------------------------------------------------------
void MIGHTY::Script::runScript( void )
{
	if( mbLoaded )
	{
		int s = lua_pcall( mlState, 0, LUA_MULTRET, 0 );

		if( s > 0 )
		{
			std::string errorMsg;

			errorMsg = lua_tostring( mlState, -1 );

			mssOut.str( "" );
			mssOut << "Script::runScript : Error caught running script "
				<< sScriptName << "\n"
				<< "    Error code is " << getLuaErrorString( s ) << ".\n"
				<< "    Error msg is: " << errorMsg;
			throw( ScriptException( mssOut.str().c_str() ) );
		}
	}
	else
	{
		throw( ScriptException( "Script::runScript : No script loaded yet!" ) );
	}
}

//----------------------------------------------------------
// setup
// :--> Sets up the Lua stuff so that we can load and 
//		run scripts in this object
//----------------------------------------------------------
void MIGHTY::Script::setup( void )
{
	if( mbLuaState )
	{
		lua_close( mlState );
		mbLuaState = false;
		mbLoaded   = false;
	}

	//------------------------------------------------------------
	// Create a Lua virtual machine.
	//------------------------------------------------------------
	mlState = lua_open();

	luaopen_base( mlState );
	luaopen_table( mlState );
	luaopen_io( mlState );
	luaopen_string( mlState );
	luaopen_math( mlState );

	mbLuaState = true;
	mbLoaded   = false;

	//------------------------------------------------------------
	// Set our handling function for when Lua panics
	//------------------------------------------------------------
	lua_atpanic( mlState, &(Script::catchLuaError) );

	//------------------------------------------------------------
	// Now register the C functions for the Lua scripts.
	// registerCFunctions is a virtual function which 
	// derived classes should implement.  That derived
	// class's implementation of registerCFunctions
	// will get called now:
	//------------------------------------------------------------
	registerCFunctions();

	//------------------------------------------------------------
	// Now clear the stack.  Various things are left on the stack
	// from the above operations, and we don't need them.  It
	// is perfectly acceptable (and correct?) to pop them off
	// so that the stack is empty.
	//------------------------------------------------------------
	int numPops = lua_gettop( mlState );
	lua_pop( mlState, numPops );
}
    
//----------------------------------------------------------------------
// callFunction
// :--> Code from "Programming in Lua"
//		http://www.lua.org/pil/25.3.html
//
//		MIGHTY modification:
//			String results need to be handled with std::string* 
//			arguments.  When Lua returns a string value to C, it leaves
//			the string on the stack and it is C's responsibility to
//			pop the strings off the stack.  So, this function will 
//			copy the string result into a given std::string and then
//			pop the string off the stack.
//			This is a change from the PiL book's source.
//----------------------------------------------------------------------
void MIGHTY::Script::callFunction( const char *func, const char *sig, ...)
{
	if( !mbLoaded )
	{
		throw( MIGHTY::ScriptException( 
			"Script::callFunction : A script is not loaded!" ) );
	}

	lua_State *L = mlState;

    va_list vl;
    int narg, nres;  /* number of arguments and results */
	
	char *			pStringOutput = NULL;
	std::string *	pString = NULL;
	int				numPopsNeeded = 0;

    va_start(vl, sig);
    lua_getglobal(L, func);  /* get function */

	if( lua_isfunction( L, -1 ) )
	{
		/* push arguments */
		narg = 0;
		while (*sig) {  /* push arguments */
		switch (*sig++) {

			case 'd':  /* double argument */
			lua_pushnumber(L, va_arg(vl, double));
			break;

			case 'i':  /* int argument */
			lua_pushnumber(L, va_arg(vl, int));
			break;

			case 's':  /* string argument */
			lua_pushstring(L, va_arg(vl, char *));
			break;

			case '>':
			goto endwhile;

			default:
				mssOut.str( "" );
				mssOut << "Script::callFunction : invalid option ("
					<< *(sig - 1) << ")";
				throw( MIGHTY::ScriptException( mssOut.str().c_str() ) );
		}
		narg++;
		luaL_checkstack(L, 1, "too many arguments");
		} endwhile:

		/* do the call */
		nres = strlen(sig);  /* number of expected results */
		if (lua_pcall(L, narg, nres, 0) != 0)  /* do the call */
		{
			mssOut.str( "" );
			mssOut << "Script::callFunction : error running function "
				<< func << " : " << lua_tostring(L, -1);
			throw( MIGHTY::ScriptException( mssOut.str().c_str() ) );
		}

		/* retrieve results */
		nres = -nres;  /* stack index of first result */
		while (*sig) {  /* get results */
		switch (*sig++) {

			case 'd':  /* double result */
				if (!lua_isnumber(L, nres))
				{
					throw( MIGHTY::ScriptException( "Script::callFunction : wrong result type" ) );
				}
				*va_arg(vl, double *) = lua_tonumber(L, nres);
				break;

			case 'i':  /* int result */
				if (!lua_isnumber(L, nres))
				{
					throw( MIGHTY::ScriptException( "Script::callFunction : wrong result type" ) );
				}

				*va_arg(vl, int *) = (int)lua_tonumber(L, nres);
				break;

			case 's':  /* string result */
				if (!lua_isstring(L, nres))
				{
					throw( MIGHTY::ScriptException( "Script::callFunction : wrong result type" ) );
				}
				*va_arg(vl, std::string *) = lua_tostring(L, nres);
				numPopsNeeded++;
				break;

			default:
				mssOut.str( "" );
				mssOut << "Script::callFunction : invalid option ("
					<< *(sig - 1) << ")";
				throw( MIGHTY::ScriptException( mssOut.str().c_str() ) );
		}
		nres++;
		}
	}

	// Pop string results now.
	lua_pop( L, numPopsNeeded );

    va_end(vl);
}

//-----------------------------------------------------------
// addStringGlobal
// :--> Add a global string variable to the script's 
//		environment.  You can only do this after a script
//		has been loaded and run.
//-----------------------------------------------------------
void MIGHTY::Script::addStringGlobal( const std::string& aName,
									  const std::string& aValue )
{
	addStringGlobal( mlState, aName, aValue );
}

void MIGHTY::Script::addStringGlobal( lua_State *L, const std::string& aName,
									  const std::string& aValue )
{
	lua_pushstring( L, aValue.c_str() );
	lua_setglobal( L, aName.c_str() );
}

//-----------------------------------------------------------
// addNumberGlobal
// :--> Add a global number (float) variable to the script's 
//		environment.  You can only do this after a script
//		has been loaded and run.
//-----------------------------------------------------------
void MIGHTY::Script::addNumberGlobal( const std::string& aName,
									  double aValue )
{
	addNumberGlobal( mlState, aName, aValue );
}

void MIGHTY::Script::addNumberGlobal( lua_State *L,const std::string& aName,
									  double aValue )
{
	lua_pushnumber( L, aValue );
	lua_setglobal( L, aName.c_str() );
}
//-----------------------------------------------------------
// getStringGlobal
// :--> Get a string global from the script's environment.
//-----------------------------------------------------------
const std::string& MIGHTY::Script::getStringGlobal( const std::string& aName )
{
	return( getStringGlobal( mlState, aName ) );
}

const std::string& MIGHTY::Script::getStringGlobal( lua_State *L, const std::string& aName )
{
	static std::string sOutput;

	lua_getglobal( L, aName.c_str() );

	if( lua_isstring( L, -1 ) )
	{
		sOutput = lua_tostring( L, -1 );
	}
	else
	{
		sOutput = "";
	}

	lua_pop( L, 1 );

	return( sOutput );
}

//-----------------------------------------------------------
// getNumberGlobal
// :--> Get a number global from the script's environment.
//-----------------------------------------------------------
double MIGHTY::Script::getNumberGlobal( const std::string& aName )
{
	return( getNumberGlobal( mlState, aName ) );
}

double MIGHTY::Script::getNumberGlobal( lua_State *L, const std::string& aName )
{
	static double fOutput;

	lua_getglobal( L, aName.c_str() );

	if( lua_isnumber( L, -1 ) )
	{
		fOutput = (double)lua_tonumber( L, -1 );
	}
	else
	{
		fOutput = 0;
	}

	lua_pop( L, 1 );

	return( fOutput );
}

//-----------------------------------------------------------
// getBoolGlobal
// :--> Get a boolean global from the script's environment.
//-----------------------------------------------------------
bool MIGHTY::Script::getBoolGlobal( const std::string& aName )
{
	return( getBoolGlobal( mlState, aName ) );
}

bool MIGHTY::Script::getBoolGlobal( lua_State *L, const std::string& aName )
{
	static bool bOutput;

	lua_getglobal( L, aName.c_str() );

	if( lua_isboolean( L, -1 ) )
	{
		bOutput = (bool)lua_toboolean( L, -1 );
	}
	else
	{
		bOutput = false;
	}

	lua_pop( L, 1 );

	return( bOutput );
}


//-----------------------------------------------------------
// addStringGlobalToTable
// :--> Add a global string variable to the script's 
//		environment in an existing table.
//		You can only do this after a script has been loaded
//		and run.
//-----------------------------------------------------------
void MIGHTY::Script::addStringGlobalToTable(
									const std::string& aTable,
									const std::string& aName,
									const std::string& aValue)
{
	addStringGlobalToTable( mlState, aTable, aName, aValue );
}

void MIGHTY::Script::addStringGlobalToTable(lua_State *L,
									const std::string& aTable,
									const std::string& aName,
									const std::string& aValue)
{
	lua_getglobal(  L, aTable.c_str() );
	lua_pushstring( L, aName.c_str() );
	lua_pushstring( L, aValue.c_str() );
	lua_settable(   L, -3 );

	lua_pop( L, 1 );
}

//-----------------------------------------------------------
// addNumberGlobalToTable
// :--> Add a global number variable to the script's 
//		environment in an existing table.
//		You can only do this after a script has been loaded
//		and run.
//-----------------------------------------------------------
void MIGHTY::Script::addNumberGlobalToTable(
								const std::string& aTable,
								const std::string& aName,
								double aValue )
{
	addNumberGlobalToTable( mlState, aTable, aName, aValue );
}

void MIGHTY::Script::addNumberGlobalToTable(lua_State *L,
								const std::string& aTable,
								const std::string& aName,
								double aValue )
{
	lua_getglobal(  L, aTable.c_str() );
	lua_pushstring( L, aName.c_str() );
	lua_pushnumber( L, aValue );
	lua_settable(   L, -3 );

	lua_pop( L, 1 );
}

//-----------------------------------------------------------
// getStringGlobalFromTable
// :--> Get a global string variable from the script's 
//		environment in an existing table.
//		You can only do this after a script has been loaded
//		and run.
//-----------------------------------------------------------
const std::string& MIGHTY::Script::getStringGlobalFromTable(
								const std::string& aTable,
								const std::string& aName )
{
	try
	{
		return( getStringGlobalFromTable( mlState, aTable, aName ) );
	}
	catch( MIGHTY::ScriptException e )
	{
		mssOut << "    script name = " << sScriptName << "\n";

		throw( MIGHTY::ScriptException( mssOut.str().c_str() ) );
	}
}

const std::string& MIGHTY::Script::getStringGlobalFromTable(lua_State *L,
								const std::string& aTable,
								const std::string& aName )
{
	static std::string	returnString;

	lua_getglobal(  L, aTable.c_str() );
	lua_pushstring( L, aName.c_str() );
	lua_gettable( L, -2 );  // table "me" is at position -2 on stack

	if( lua_isstring( L, -1 ) )
	{
		returnString = lua_tostring( L, -1 );
		lua_pop( L, 2 );

		return( returnString );
	}
	else
	{
		lua_pop( L, 2 );

		mssOut.str("");
		mssOut << "Script::getStringGlobalFromTable : Not a string!\n"
			<< "    table name  = " << aTable << "\n"
			<< "    key name    = " << aName << "\n";

		throw( MIGHTY::ScriptException( mssOut.str().c_str() ) );
	}
}

//-----------------------------------------------------------
// getStringGlobalFromTable
// :--> Get a global string variable from the script's 
//		environment in an existing table.
//		You can only do this after a script has been loaded
//		and run.
//-----------------------------------------------------------
const std::string& MIGHTY::Script::getStringGlobalFromTable(
								const std::string& aTable,
								const int		   aIndex )
{
	try
	{
		return( getStringGlobalFromTable( mlState, aTable, aIndex ) );
	}
	catch( MIGHTY::ScriptException e )
	{
		mssOut << "    script name = " << sScriptName << "\n";

		throw( MIGHTY::ScriptException( mssOut.str().c_str() ) );
	}
}

const std::string& MIGHTY::Script::getStringGlobalFromTable(lua_State *L,
								const std::string&  aTable,
								const int			aIndex )
{
	static std::string	returnString;

	lua_getglobal(  L, aTable.c_str() );
	lua_pushnumber( L, aIndex );
	lua_gettable( L, -2 );  

	if( lua_isstring( L, -1 ) )
	{
		returnString = lua_tostring( L, -1 );
		lua_pop( L, 2 );

		return( returnString );
	}
	else
	{
		lua_pop( L, 2 );

		mssOut.str("");
		mssOut << "Script::getStringGlobalFromTable : Not a string!\n"
			<< "    table name  = " << aTable << "\n"
			<< "    key         = " << aIndex << "\n";

		throw( MIGHTY::ScriptException( mssOut.str().c_str() ) );
	}
}

//-----------------------------------------------------------
// addNumberGlobalToTable
// :--> Get a global number variable from the script's 
//		environment in an existing table.
//		You can only do this after a script has been loaded
//		and run.
//-----------------------------------------------------------
double MIGHTY::Script::getNumberGlobalFromTable(
								const std::string& aTable,
								const std::string& aName )
{
	try
	{
		return( getNumberGlobalFromTable( mlState, aTable, aName ) );
	}
	catch( MIGHTY::ScriptException e )
	{
		mssOut << "    script name = " << sScriptName << "\n";

		throw( MIGHTY::ScriptException( mssOut.str().c_str() ) );
	}
}

double MIGHTY::Script::getNumberGlobalFromTable(lua_State *L,
								const std::string& aTable,
								const std::string& aName )
{
	static double dValue;

	lua_getglobal(  L, aTable.c_str() );
	lua_pushstring( L, aName.c_str() );
	lua_gettable( L, -2 );  // table "me" is at position -2 on stack

	if( lua_isnumber( L, -1 ) )
	{
		dValue = lua_tonumber( L, -1 );
		lua_pop( L, 2 );

		return( dValue );
	}
	else
	{
		lua_pop( L, 2 );

		mssOut.str("");
		mssOut << "Script::getStringGlobalFromTable : Not a number!\n"
			<< "    table name  = " << aTable << "\n"
			<< "    key name    = " << aName << "\n";

		throw( MIGHTY::ScriptException( mssOut.str().c_str() ) );
	}
}

//-----------------------------------------------------------
// globalExists
// :--> Return true if a global variable exists with the
//		given name.
//-----------------------------------------------------------
bool MIGHTY::Script::globalExists( const std::string& aName )
{
	try
	{
		return( globalExists( mlState, aName ) );
	}
	catch( MIGHTY::Exception e )
	{
		mssOut.str( "" );
		mssOut << "MIGHTY::Script::globalExists : Failed for script " << sScriptName << ":\n"
			<< e.getErrorText();

		throw( MIGHTY::ScriptException( mssOut.str().c_str() ) );
	}
}

bool MIGHTY::Script::globalExists( lua_State *L, const std::string& aName )
{
	return( globalExistsInTable( L, "_G", aName ) );
}

//-----------------------------------------------------------
// globalExistsInTable
// :--> Return true if a global variable exists with the
//		given name in a given table.
//-----------------------------------------------------------
bool MIGHTY::Script::globalExistsInTable( const std::string& aTable,
							const std::string& aName )
{
	try
	{
		return( globalExistsInTable( mlState, aTable, aName ) );
	}
	catch( MIGHTY::Exception e )
	{
		mssOut.str( "" );
		mssOut << "MIGHTY::Script::globalExistsInTable : Failed for script " << sScriptName << ":\n"
			<< e.getErrorText();

		throw( MIGHTY::ScriptException( mssOut.str().c_str() ) );
	}
}

bool MIGHTY::Script::globalExistsInTable( lua_State *L, const std::string& aTable,
							const std::string& aName )
{
	// 1. Put the table on the top of the stack:
	lua_getglobal(  L, aTable.c_str() );

	// 2. Put the index key in the table onto the stack:
	lua_pushstring( L, aName.c_str() );

	// 3. Get aTable[ aName ] and put it on top of the stack.
	//    The value pushed onto the stack in step 2 is popped,
	//    but the table value pushed in step 1 remains.
	lua_gettable(   L, -2 );

	// We now have two items on the stack:
	// 1. The "aTable" table
	// 2. The value of aTable[ aName ], which is on the stack's top.

	if( lua_type( L, -1 ) != LUA_TNIL )
	{
		lua_pop( L, 2 );
		return( true );
	}
	else
	{
		lua_pop( L, 2 );
		return( false );
	}
}


bool MIGHTY::Script::globalExistsInTable( const std::string& aTable,
								  const int aIndex )
{
	try
	{
		return( globalExistsInTable( mlState, aTable, aIndex ) );
	}
	catch( MIGHTY::Exception e )
	{
		mssOut.str( "" );
		mssOut << "MIGHTY::Script::globalExistsInTable : Failed for script " << sScriptName << ":\n"
			<< e.getErrorText();

		throw( MIGHTY::ScriptException( mssOut.str().c_str() ) );
	}
}

bool MIGHTY::Script::globalExistsInTable(lua_State *L,  const std::string& aTable,
								  const int aIndex )
{
	// 1. Put the table on the top of the stack:
	lua_getglobal(  L, aTable.c_str() );

	// 2. Put the index key in the table onto the stack:
	lua_pushnumber( L, aIndex );

	// 3. Get aTable[ aName ] and put it on top of the stack.
	//    The value pushed onto the stack in step 2 is popped,
	//    but the table value pushed in step 1 remains.
	lua_gettable(   L, -2 );

	// We now have two items on the stack:
	// 1. The "aTable" table
	// 2. The value of aTable[ aName ], which is on the stack's top.

	if( lua_type( L, -1 ) != LUA_TNIL )
	{
		lua_pop( L, 2 );
		return( true );
	}
	else
	{
		lua_pop( L, 2 );
		return( false );
	}
}

//-----------------------------------------------------------
// stackDump
// :-->	Print out the contents of the Lua stack into a 
//		string.
//-----------------------------------------------------------
const std::string& MIGHTY::Script::stackDump( void )
{
	return( stackDump( mlState ) );
}

const std::string& MIGHTY::Script::stackDump( lua_State *L )
{
	int i;
	int top = lua_gettop(L);
	static std::string	sOutput;

	mssOut.str( "" );

	for (i = 1; i <= top; i++) 
	{  
		/* repeat for each level */
		int t = lua_type(L, i);

		switch (t) 
		{
			case LUA_TSTRING:  /* strings */
				mssOut << "'" << lua_tostring(L, i) << "'";
				break;

			case LUA_TBOOLEAN:  /* booleans */
				mssOut << (lua_toboolean(L, i) ? "true" : "false");
				break;

			case LUA_TNUMBER:  /* numbers */
				mssOut << lua_tonumber(L, i);
				break;

			default:  /* other values */
				mssOut << lua_typename(L, t);
				break;
		}

		mssOut << "  ";  /* put a separator */
	}

	mssOut << std::endl;  /* end the listing */

	sOutput = mssOut.str();

	return( sOutput );
}
