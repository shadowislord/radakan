#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.50)

AC_INIT( [OIS], 0.7.1 )

AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE( [OIS], 0.7.1 )
AM_CONFIG_HEADER([includes/config.h])

dnl Check for programs
AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_INSTALL
AM_PROG_LIBTOOL

dnl Checking for STLPort
OIS_USE_STLPORT

dnl Checking for building of CEGUIOgre ActionMap Demo
OIS_ENABLE_OGRE_SAMPLE

dnl See if we are using oldschool JoyStick API or newer Event API system
OIS_JOYSTICK_EVENTS

CFLAGS="$CFLAGS"
CXXFLAGS="$CXXFLAGS"
LIBS="$LIBS"

dnl Detect X11
AC_CHECK_HEADERS([X11/Xaw/Command.h],, [AC_MSG_ERROR("libxaw headers not found")])

dnl Added for BSD's
AC_PROG_LIBTOOL

AC_CONFIG_FILES([Makefile
                 includes/Makefile
                 src/Makefile
		 demos/Makefile
		 demos/ogre/Makefile
		 OIS.pc])
AC_OUTPUT
