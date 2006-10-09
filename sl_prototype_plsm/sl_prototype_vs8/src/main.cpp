#include <Ogre.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#include "input.hpp"
#include "tslrpg.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
   INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
   int main(int argc, char *argv[])
#endif
   {
      // Create application object
      Tslrpg_Prototype tsl_prototype;

      SET_TERM_HANDLER;

      try {

         tsl_prototype.run();

      } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
         MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
         std::cerr << "An exception has occured: " <<
            e.getFullDescription().c_str() << std::endl;
#endif
      }

      return 0;
   }

#ifdef __cplusplus
}
#endif

