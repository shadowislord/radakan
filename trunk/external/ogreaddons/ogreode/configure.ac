# Modified the 06/23/2006 by
# Martin Pieuchot (Aer) <t1tamar (at) gmail.com>

AC_PREREQ(2.59)
AC_INIT([OgreOde_Core], [0.7.10])
AM_INIT_AUTOMAKE([OgreOde_Core], [0.7.10])
AM_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([foreign dist-bzip2])

dnl Checks for programs
AC_PROG_CXX
AC_PROG_LIBTOOL

PKG_CHECK_MODULES(OGRE, [OGRE >= 1.2.0],,
			  AC_MSG_ERROR([OGRE is required to compile Ogre-Ode]))
AC_SUBST(OGRE_CFLAGS)
AC_SUBST(OGRE_LIBS)

PKG_CHECK_MODULES(ois, [OIS >= 0.5.0],,
			  AC_MSG_ERROR([OIS is required to compile Ogre-Ode samples]))
AC_SUBST(ois_CFLAGS)
AC_SUBST(ois_LIBS)

AC_CHECK_HEADERS([ode/ode.h],,
			  AC_MSG_ERROR([ODE is required to compile Ogre-Ode]))


AC_ARG_WITH([samples],AC_HELP_STRING([--with-samples=[PATH]],
	    [look for the ogre samples in PATH, default is
	     ../../../../../ogrenew/Samples/]),
	     [samples=$withval],[samples=no])

SAMPLES="../../../../../ogrenew/Samples"
if test "x$samples" != "xno"
then
	SAMPLES="${samples}"
fi
SAMPLES_CFLAGS="${SAMPLES}/Common/include"
AC_SUBST(SAMPLES)
AC_SUBST(SAMPLES_CFLAGS)
AC_CONFIG_FILES([Makefile 
	src/Makefile 
	include/Makefile 
	OgreOde_Core.pc 
	prefab/Makefile
	prefab/include/Makefile 
	prefab/src/Makefile 
	OgreOde_Prefab.pc
	OgreOde_Loader.pc
	loader/Makefile
	loader/src/Makefile
	demos/Makefile 
	demos/SimpleScenes/Makefile 
	demos/SimpleScenes/src/Makefile
	demos/GranTurismOgre/Makefile
	demos/GranTurismOgre/src/Makefile
	demos/Landscape/Makefile
	demos/Landscape/src/Makefile
	])

AC_OUTPUT
