/*
 * tslrpg.h
 *
 * Master include file for The Scattered Lands.
 * All other files should include this one.
 */

#ifndef __tslrpg_h
#define __tslrpg_h

/* We expect to use Crystal Space everywhere. */
#include <cssysdef.h>

/* MSVC users do not run configure, so use special MSVC configuration file. */
#if defined(CS_WIN32_CSCONFIG)
#include "config-msvc.h"
#else
#include "config.h"
#endif

/* Insert additional project-specific declarations here. */

#endif // __tslrpg_h
