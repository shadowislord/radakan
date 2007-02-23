#build exe tslrpg
find_package(Fmod)
find_package(Ogre)
find_package(Boost)
find_package(DIrectX)

file(GLOB_RECURSE SRC_FILES "${TSLRPG_SOURCE_DIR}/src/cpp" *.cpp)

set(INCLUDE_DIRS    "${TSLRPG_SOURCE_DIR}/src/hpp"
                    ${Boost_INCLUDE_DIR}
                    "${TSLRPG_SOURCE_DIR}/external/ode-0.8/include"
                    "${TSLRPG_SOURCE_DIR}/external/ogreaddons/ogreode/include"
                    "${TSLRPG_SOURCE_DIR}/external/ogreaddons/ogreode/prefab/include"
                    "${TSLRPG_SOURCE_DIR}/external/ogreaddons/ogreode/loader/include"
                    "${TSLRPG_SOURCE_DIR}/external/ogreaddons/ogreode/tinyxml-2.3.3"
                    ${Fmod_INCLUDE_DIR}
                    "${TSLRPG_SOURCE_DIR}/external/ois-1.0RC1/includes"
                    ${Ogre_INCLUDE_DIR}
                    ${Ogre_INCLUDE_DIR}/CEGUI
                    ${Ogre_SAMPLES_DIR}/include
                    )

set(LINK_DIRS       ${Ogre_LIB_RELEASE} 
                    ${Fmod_LIB_RELEASE}
                    ${DirectX_LIB_DIR}
                    ${TSLRPG_BINARY_DIR}/odeogre_core
                    ${TSLRPG_BINARY_DIR}/tinyxml
                    ${TSLRPG_BINARY_DIR}/prefab
                    ${TSLRPG_BINARY_DIR}/loader                    
                    ${TSLRPG_BINARY_DIR}/ois)

if(WIN32)
    link_directories(  ${LINK_DIRS}  ${Directx_INCLUDE_DIR} ) 
    add_executable( tslrpg WIN32 ${SRC_FILES} )
else(WIN32)
    link_directories(  ${LINK_DIRS} )         
    add_executable( tslrpg ${SRC_FILES} )
endif(WIN32)                

include_directories( ${INCLUDE_DIRS} )                


set(DEBUG_LIBS  debug ogremain_d 
                debug ceguibase_d 
                debug ogreguirenderer_d 
                debug tinyxml 
                debug prefab 
                debug ogreode_core 
                debug ois 
                debug fmodvc)
set(RELEASE_LIBS    optimized ogremain 
                    optimized ceguibase 
                    optimized ogreguirenderer 
                    optimized tinyxml 
                    optimized prefab 
                    optimized ogreode_core  
                    optimized ois  
                    optimized fmodvc)
target_link_libraries( tslrpg   ${RELEASE_LIBS}
                                ${DEBUG_LIBS} )


install( TARGETS tslrpg DESTINATION ${TSLRPG_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
install( TARGETS tslrpg DESTINATION ${TSLRPG_SOURCE_DIR}/bin/release CONFIGURATIONS Release )
