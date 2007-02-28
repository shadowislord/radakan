cmake_minimum_required(VERSION 2.0)
project(TSLRPG)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${TSLRPG_SOURCE_DIR}/cmake)

subdirs(    "external/ois-1.0RC1" 
            "external/ode-0.8" 
            "external/ogreaddons/ogreode" 
            "external/audiere" 
            "src" )

install( DIRECTORY ${TSLRPG_SOURCE_DIR}/data DESTINATION ${TSLRPG_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug ) 
install( DIRECTORY ${TSLRPG_SOURCE_DIR}/data DESTINATION ${TSLRPG_SOURCE_DIR}/bin/release CONFIGURATIONS Release ) 

