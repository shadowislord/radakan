#build exe tslrpg
find_package(Fmod)
find_package(Ogre)
find_package(Boost)

if(WIN32)
    find_package(DIrectX)
endif(WIN32)

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
                    "${TSLRPG_SOURCE_DIR}/external/audiere/include"
                    ${Ogre_INCLUDE_DIR}
                    ${Ogre_INCLUDE_DIR}/CEGUI
                    ${Ogre_SAMPLES_DIR}/include
                    )

set(LINK_DIRS       ${Ogre_LIB_RELEASE} 
                    ${Fmod_LIB_RELEASE}
                    ${TSLRPG_BINARY_DIR}/odeogre_core
                    ${TSLRPG_BINARY_DIR}/audiere
                    ${TSLRPG_BINARY_DIR}/tinyxml
                    ${TSLRPG_BINARY_DIR}/prefab
                    ${TSLRPG_BINARY_DIR}/loader                    
                    ${TSLRPG_BINARY_DIR}/ois)
               

if(WIN32)
    set(LINK_DIRS ${LINK_DIRS} ${DirectX_LIB_DIR})
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
                debug fmodvc
                debug audiere)
set(RELEASE_LIBS    optimized ogremain 
                    optimized ceguibase 
                    optimized ogreguirenderer 
                    optimized tinyxml 
                    optimized prefab 
                    optimized ogreode_core  
                    optimized ois  
                    optimized fmodvc
                    optimized audiere)
target_link_libraries( tslrpg   ${RELEASE_LIBS}
                                ${DEBUG_LIBS} )

install( TARGETS tslrpg DESTINATION ${TSLRPG_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
install( TARGETS tslrpg DESTINATION ${TSLRPG_SOURCE_DIR}/bin/release CONFIGURATIONS Release )
