
AC_DEFUN([OIS_USE_STLPORT],
[AC_ARG_WITH(stlport, 
             AC_HELP_STRING([--with-stlport=PATH],
                           [the path to STLPort.]),
             ac_cv_use_stlport=$withval,
             ac_cv_use_stlport=no)
 AC_CACHE_CHECK([whether to use STLPort], ac_cv_use_stlport,
                ac_cv_use_stlport=no)
 if test x$ac_cv_use_stlport != xno; then
     STLPORT_CFLAGS="-I$ac_cv_use_stlport/stlport"
     STLPORT_LIBS="-L$ac_cv_use_stlport/lib -lstlport"
 fi
 AC_SUBST(STLPORT_CFLAGS)
 AC_SUBST(STLPORT_LIBS)
])

AC_DEFUN([OIS_ENABLE_OGRE_SAMPLE], [
    AC_ARG_ENABLE(ogre,
        AC_HELP_STRING([--disable-ogre], 
            [Disables the build of the OgreCEGUI ActionMap Demo]),
            [build_ogre=$enableval], [build_ogre=yes])

    if test "x$build_ogre" = "xyes" ; then
        PKG_CHECK_MODULES(OGRE, OGRE >= 1.0.5, [ois_found_ogre=yes],[cegui_found_ogre=no])
        AC_SUBST(OGRE_CFLAGS)
        AC_SUBST(OGRE_LIBS)

        PKG_CHECK_MODULES(CEGUI, CEGUI >= 0.4.0, [ois_found_cegui=yes],[ois_found_cegui=no])
        AC_SUBST(CEGUI_CFLAGS)
        AC_SUBST(CEGUI_LIBS)

        if test "x$ois_found_ogre" = "xyes" && test "x$ois_found_cegui" = "xyes" ; then
           ois_samples_use_ogre=yes
            AC_MSG_NOTICE([Use of Ogre3D in Samples is Enabled])
        else
            ois_samples_use_ogre=no
            AC_MSG_NOTICE([Ogre Demo Disabled])
        fi

    else
        ois_samples_use_ogre=no
        AC_MSG_NOTICE([Ogre Demo Disabled])
    fi

    AM_CONDITIONAL([OIS_BUILD_OGRE_SAMPLE], [test x$ois_samples_use_ogre = xyes])
])

AC_DEFUN([OIS_JOYSTICK_EVENTS], [
    AC_ARG_ENABLE(joyevents,
        AC_HELP_STRING([--disable-joyevents], 
            [Uses /dev/input/js# instead of the events /dev/input/event#]),
            [build_joyevents=$enableval], [build_joyevents=yes])

    if test "x$build_joyevents" = "xyes" ; then
        ois_joyevent_use=yes
        AC_MSG_NOTICE([Using /dev/input/eventXX for JoyStick.. Force Feedback support enabled])
        AC_DEFINE(OIS_JOYEVENTS,,[Building with input.h Joystick support (instead of Joystick.h)])
    else
        ois_joyevent_use=no
        AC_MSG_NOTICE([Using /dev/input/jsXX for JoyStick.. Force Feedback support **disabled**])
    fi

    AM_CONDITIONAL([OIS_BUILD_LINUX_EVENTS], [test x$ois_joyevent_use = xyes])
])
