cmake_minimum_required(VERSION 2.0)
project(TSLRPG)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${TSLRPG_SOURCE_DIR}/cmake)

subdirs( "external/ois-1.0RC1" "external/ogreaddons/ogreode" )
