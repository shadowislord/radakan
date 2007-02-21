#build loader library

find_package(Ogre)
find_package(Ode)

set(SRC_FILES   "src/OgreOdeDotLoader.cpp")

set(INCLUDE_DIRS    "${TSLRPG_SOURCE_DIR}/external/ogreaddons/ogreode/include"
                    "${TSLRPG_SOURCE_DIR}/external/ogreaddons/ogreode/prefab/include"
                    ${Ogre_INCLUDE_DIR}
                    ${Ode_INCLUDE_DIR})            

include_directories( ${INCLUDE_DIRS} )                
link_directories(   ${Ogre_LIB_DEBUG} 
                    ${Ode_LIB_DEBUG} 
                    ${Ogre_LIB_RELEASE} 
                    ${Ode_LIB_RELEASE} 
                    ${TSLRPG_BINARY_DIR}/odeogre_core
                    ${TSLRPG_BINARY_DIR}/tinyxml)
add_library( loader SHARED ${SRC_FILES} )
target_link_libraries( loader   optimized ode ogremain tinyxml ogreode_core
                                debug ode ogremain_d tinyxml ogreode_core )
