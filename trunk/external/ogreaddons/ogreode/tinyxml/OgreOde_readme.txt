TinyXml for OgreOde
===================

To build TinyXml for use with OgreOde you should be able to use the supplied 
MSVC projects. The only change you need to make is that you must change the Code 
Generation option to "Multi-threaded DLL" for Release builds, or 
"Multi-threaded Debug DLL" for Debug builds.

The only project you are required to build is the TinyXml one, the STL version 
and the test programs are not required. The TinyXml libraries should be placed 
under the Debug and Release subdirectories of this one, which should be 
$OGRE_TOP/OgreOde/tinyxml, if you are compiling using an environment other than 
MSVC then you will need to place the libraries in a place which is accessible to 
you OgreOde build environment.

