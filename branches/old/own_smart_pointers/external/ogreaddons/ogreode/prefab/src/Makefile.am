INCLUDES = \
   $(STLPORT_CFLAGS) \
   ${OGRE_CFLAGS} \
   -I$(top_srcdir)/include \
   -I../include \
   -I$(top_srcdir)/tinyxml


lib_LTLIBRARIES = libOgreOde_Prefab.la

libOgreOde_Prefab_la_SOURCES = \
   OgreOdeVehicle.cpp \
   OgreOdeRagdoll.cpp \
   OgreOdePrefabObject.cpp \
   OgreOdeObject.cpp \
   $(top_srcdir)/tinyxml/tinystr.cpp \
   $(top_srcdir)/tinyxml/tinyxml.cpp \
   $(top_srcdir)/tinyxml/tinyxmlerror.cpp \
   $(top_srcdir)/tinyxml/tinyxmlparser.cpp


libOgreOde_Prefab_la_LIBADD = \
   $(OGRE_LIBS) \
   -L$(top_srcdir)/src \
   -lOgreOde_Core \
   -lode 
