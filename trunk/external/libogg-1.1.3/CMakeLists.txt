#build lib ogg
set( SRC_FILES  "src/bitwise.c"
                "src/framing.c")

set( INCLUDE_DIRS  "${RADAKAN_SOURCE_DIR}/external/libogg-1.1.3/include" )

include_directories( ${INCLUDE_DIRS} )
add_library( ogg  ${SRC_FILES} )
