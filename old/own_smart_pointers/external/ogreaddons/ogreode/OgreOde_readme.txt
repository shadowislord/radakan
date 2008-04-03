Readme for OgreOde
==================

DOWNLOADED ZIP VERSION
======================
Brief build instructions follow for OgreOde 1.2.0 Final Edition. 
Consider this version of OgreOde deprecated. A new and exciting development
is on the horizon. Watch this space.

1) You should have extracted this archive next to $OGRE_TOP, where $OGRE_TOP
   is the root directory of your Ogre installation, usually named 'ogresdk'.
   All file paths in this build environment should be relative and all the DLLs
   and executables should get sent to the correct place upon building.

   This version of OgreOde is only supported when compiled against version 
   1.2.x final of the Ogre library aka Dagon,.

2) ODE is available from http://www.ode.org/ It requires that you get Latest ODE version
 which is actually available only in ode subversion repository.

3) Version 2.3.3 of TinyXml is included in the tinyxml directory. See
   the OgreOde_readme.txt file in that directory for further details, especially 
   regarding the changes to the MSVC projects which are required for the library 
   to be compatible with OgreOde. 

   TinyXml is only required by the OgreOde_Prefabs library, if you are not using 
   this (i.e. you don't need the supplied Vehicle and Ragdoll functionality) then 
   you do not need to build TinyXml.

   TinyXml is available from http://www.grinninglizard.com/tinyxml/

4) A crash when exiting the Landscape demo is known and will be fixed in Ogre soon.
It's related to CustomMaterial use and has no link with OgreOde


5) Use the solution OgreOde/scripts/VC8 and OgreOde/scripts/VC7.1 (thanks to PxL)
   to build the core OgreOde library, the OgreOde prefabricated objects library 
   and the suite of demo programs with Microsoft Visual C++ 2005 or Microsoft 
   Visual C++ 2003. The DLLs and executables will be built to the $OGRE_TOP/bin/Debug
   and Release directories.

   Simply open up that solution and perform a batch build.

6) There are some scripts which should build OgreOde under Linux in the scripts/Linux
   directory. They are provided courtesy of Pablo, please see the Readme in that directory
   for further details.

Post On Ogre Forums for details.

CVS VERSION
======================
Brief build instructions follow for OgreOde 1.2.0 Final Edition. 
Consider this version of OgreOde deprecated. A new and exciting development
is on the horizon. Watch this space.

1) This version of OgreOde is only supported when compiled against version 
   1.2.X branch of the Ogre library aka Dagon,.

2) you need to get ODE library 
   ODE download instruction and communauty is available from http://www.ode.org/


5) Use the _CVS.sln solution OgreOde/scripts/VC8 
   to build the core OgreOde library, the OgreOde prefabricated objects library 
   and the suite of demo programs with Microsoft Visual C++ 2005 or Microsoft 
   Visual C++ 2003. The DLLs and executables will be built to the ..\..\ogrenew\Samples\common/bin/Debug
   and Release directories.

   Simply open up that solution and perform a batch build.

6) There are some scripts which should build OgreOde under Linux in the scripts/Linux
   directory. They are provided courtesy of Pablo, please see the Readme in that directory
   for further details.

Post On Ogre Forums for details.
