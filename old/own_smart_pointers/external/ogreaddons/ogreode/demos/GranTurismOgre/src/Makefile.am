noinst_PROGRAMS = GranTurismOgre

GranTurismOgre_CXXFLAGS = \
   ${OGRE_CFLAGS} \
   -I${SAMPLES_CFLAGS} \
   -I../include \
   -I$(top_srcdir)/include \
   -I$(top_srcdir)/prefab/include \
   -I$(top_srcdir)/loader/include \
   -I$(top_srcdir)/tinyxml 
#  -I$(top_srcdir)/$(top_srcdir)renew/Samples/Common/include

GranTurismOgre_LDFLAGS = \
   -L$(top_srcdir)/src \
   -L$(top_srcdir)/prefab/src \
   -L$(top_srcdir)/loader/src


GranTurismOgre_LDADD = \
   $(OGRE_LIBS) \
   -lOgreOde_Core \
   -lOgreOde_Prefab \
   -lOgreOde_Loader


GranTurismOgre_SOURCES = \
   GranTurismOgre.cpp \
   $(top_srcdir)/tinyxml/tinystr.cpp \
   $(top_srcdir)/tinyxml/tinyxml.cpp \
   $(top_srcdir)/tinyxml/tinyxmlerror.cpp \
   $(top_srcdir)/tinyxml/tinyxmlparser.cpp


install-exec-local:
	   $(INSTALL_PROGRAM) GranTurismOgre $(SAMPLES)/Common/bin/GranTurismOgre
