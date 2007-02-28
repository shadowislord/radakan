#build lib vorbis

set( SRC_FILES  "lib/analysis.c"
                "lib/bitrate.c"
                "lib/block.c"
                "lib/codebook.c"
                "lib/envelope.c"
                "lib/floor0.c"
                "lib/floor1.c"
                "lib/info.c"
                "lib/lookup.c"
                "lib/lpc.c"
                "lib/lsp.c"
                "lib/mapping0.c"
                "lib/mdct.c"
                "lib/psy.c"
                "lib/registry.c"
                "lib/res0.c"
                "lib/sharedbook.c"
                "lib/smallft.c"
                "lib/synthesis.c"
                "lib/window.c"
    )

set( INCLUDE_DIRS   "${TSLRPG_SOURCE_DIR}/external/libvorbis-1.1.2/include"
                    "${TSLRPG_SOURCE_DIR}/external/libogg-1.1.3/include" )                

include_directories( ${INCLUDE_DIRS} )
add_library( vorbis  ${SRC_FILES} )

if(WIN32)
    set_target_properties( vorbis PROPERTIES COMPILE_FLAGS "-DWIN32 -D_LIB" )
endif(WIN32)

set_target_properties( vorbis PROPERTIES LINKER_LANGUAGE CXX )
