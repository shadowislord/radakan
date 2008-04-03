noinst_PROGRAMS = SimpleScenes

SimpleScenes_CXXFLAGS = \
	${OGRE_CFLAGS} \
	${ois_CFLAGS} \
	-I${SAMPLES_CFLAGS} \
	-I../include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/prefab/include \
	-I$(top_srcdir)/tinyxml \
	-I$(top_srcdir)/loader/include
#	-I$(top_srcdir)/$(top_srcdir)renew/Samples/Common/include

SimpleScenes_LDFLAGS = \
	-L$(top_srcdir)/src \
	-L$(top_srcdir)/prefab/src \
	-L$(top_srcdir)/loader/src
	
SimpleScenes_LDADD = \
	$(OGRE_LIBS) \
	${ois_LIBS} \
	-lOgreOde_Core \
	-lOgreOde_Prefab \
	-lOgreOde_Loader

SimpleScenes_SOURCES = \
	SimpleScenesApplication.cpp \
	SimpleScenes.cpp \
	$(top_srcdir)/tinyxml/tinystr.cpp \
	$(top_srcdir)/tinyxml/tinyxml.cpp \
	$(top_srcdir)/tinyxml/tinyxmlerror.cpp \
	$(top_srcdir)/tinyxml/tinyxmlparser.cpp


install-exec-local:
	$(INSTALL_PROGRAM) SimpleScenes $(SAMPLES)/Common/bin/SimpleScenes

