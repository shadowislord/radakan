//	Master include file for The Scattered Lands.
//	All other files should include this one.

#ifndef TSLSYSDEF_HPP
#define TSLSYSDEF_HPP

//	We expect to use Crystal Space everywhere.
#include <cssysdef.h>

//	MSVC users do not run configure, so use special MSVC configuration file.
#if defined (CS_WIN32_CSCONFIG)
	#include "config-msvc.h"
#else
	#include "config.h"
#endif

//	Insert additional project-specific declarations below.

#endif	//	TSLSYSDEF_HPP
