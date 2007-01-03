INCLUDES = \
   $(STLPORT_CFLAGS) \
   ${OGRE_CFLAGS} \
   -I$(top_srcdir)/include
#   -I$(top_srcdir)/ode/include


lib_LTLIBRARIES = libOgreOde_Core.la


libOgreOde_Core_la_SOURCES = \
   OgreOdeBody.cpp \
   OgreOdeCollision.cpp \
   OgreOdeDebugObject.cpp \
   OgreOdeEigenSolver.cpp \
   OgreOdeEntityInformer.cpp \
   OgreOdeGeometry.cpp \
   OgreOdeJoint.cpp \
   OgreOdeMaintainedList.cpp \
   OgreOdeMass.cpp \
   OgreOdeMeshInformer.cpp \
   OgreOdeSpace.cpp \
   OgreOdeStepper.cpp \
   OgreOdeUtility.cpp \
   OgreOdeWorld.cpp

libOgreOde_Core_la_LIBADD = \
   $(OGRE_LIBS) \
   /usr/include/libode.a
