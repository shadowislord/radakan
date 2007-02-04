INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/includes $(CFLAGS) $(CEGUI_CFLAGS) $(OGRE_CFLAGS)

# An OgreCEGUI ActionMapping demo
noinst_PROGRAMS = ActionMap ForceFeedback

ActionMap_SOURCES = gui.cpp ActionConfig.cpp ActionMap.cpp
ActionMap_LDFLAGS = -L$(top_builddir)/src
ActionMap_LDADD = $(CEGUI_LIBS) $(OGRE_LIBS) -lCEGUIOgreRenderer -lOIS

ForceFeedback_SOURCES = FFgui.cpp
ForceFeedback_LDFLAGS = -L$(top_builddir)/src
ForceFeedback_LDADD = $(CEGUI_LIBS) $(OGRE_LIBS) -lCEGUIOgreRenderer -lOIS


install-exec-local:
	$(INSTALL_PROGRAM) ActionMap ./common/ActionMap
	$(INSTALL_PROGRAM) ForceFeedback ./common/ForceFeedback
