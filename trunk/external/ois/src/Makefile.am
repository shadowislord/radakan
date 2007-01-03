INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/includes $(CFLAGS) -I/usr/X11R6/include

lib_LTLIBRARIES=libOIS.la
libOIS_la_SOURCES = OISInputManager.cpp \
	OISObject.cpp \
	OISEffect.cpp \
	OISJoyStick.cpp \
	OISKeyboard.cpp \
	OISForceFeedback.cpp \
	./linux/EventHelpers.cpp \
	./linux/LinuxInputManager.cpp \
	./linux/LinuxKeyboard.cpp \
	./linux/LinuxMouse.cpp

if OIS_BUILD_LINUX_EVENTS
libOIS_la_SOURCES += ./linux/LinuxJoyStickEvents.cpp ./linux/LinuxForceFeedback.cpp
else
libOIS_la_SOURCES += ./linux/LinuxJoyStick.cpp
endif

libOIS_la_LIBFLAGS = -L/usr/X11R6/lib
libOIS_la_LIBADD = -L/usr/X11R6/lib -lX11 -lXaw
