#build prefab library

#   "src/cd_unix.cpp"           need: cdaudio.h
#   "src/cd_win32.cpp"          need: windows.h & mmsystem.h
#   "src/device_mm.cpp"         need: windows.h & mmsystem.h
#   "dxguid.cpp"                need: windows.h, mmsystem.h & dsound.h
#   "input.cpp"                 need: input_flac, input_mod and input_speex
#   "input_flac.cpp"            need: FLAC/seekable_stream_decoder.h
#   "input_mod.cpp"             need: dumb.h
#   "input_speex.cpp"           need: speex.h, speex_header.h, speex_stereo.h & speex_callbacks.h
#   "midi_mci.cpp"              need: windows.h & mmsystem.h
#   "src/mpaudec/bits.c"
#   "src/mpaudec/mpaudec.c"
#   "src/speexfile/speexfile.cpp"

find_package(Ogg)

if(NOT Ogg_FOUND)
    subdirs( "${RADAKAN_SOURCE_DIR}/external/libogg-1.1.3" )
endif(NOT Ogg_FOUND)

find_package(Vorbis)

if(NOT Vorbis_FOUND)
    subdirs( "${RADAKAN_SOURCE_DIR}/external/libvorbis-1.1.2" )
endif(NOT Vorbis_FOUND)

if(WIN32)
    find_package(DirectX)
endif(WIN32)

set(SRC_FILES
    "src/basic_source.cpp"
    "src/debug.cpp"
    "src/device.cpp"
    "src/device_mixer.cpp"
    "src/device_null.cpp"
    "src/dumb_resample.cpp"
    "src/file_ansi.cpp"
    "src/input.cpp"
    "src/input_aiff.cpp"
    "src/input_ogg.cpp"
    "src/input_wav.cpp"
    "src/loop_point_source.cpp"
    "src/memory_file.cpp"
    "src/midi_null.cpp"
    "src/noise.cpp"
    "src/resampler.cpp"
    "src/sample_buffer.cpp"
    "src/sound.cpp"
    "src/sound_effect.cpp"
    "src/square_wave.cpp"
    "src/tone.cpp"
    "src/utility.cpp"
    "src/version.cpp"
    "src/mpaudec/bits.c"
    "src/mpaudec/mpaudec.c"
)

set( INCLUDE_DIRS "" )
set( LINK_DIRS "" )

if(WIN32)
    set(SRC_FILES_WIN32     "src/audiere.rc"
                            "src/threads_win32.cpp"
                            "src/timer_win32.cpp"
                            "src/device_ds.cpp"
                            "src/device_ds_buffer.cpp"
                            "src/device_ds_stream.cpp"
                            "src/device_mm.cpp")

    set( INCLUDE_DIRS   ${INCLUDE_DIRS}
                        ${DirectX_INCLUDE_DIR} )

    set( SRC_FILES ${SRC_FILES} ${SRC_FILES_WIN32} )
else(WIN32)
    set(SRC_FILES_UNIX      "src/threads_posix.cpp"
                            "src/timer_posix.cpp"
                            "src/device_oss.cpp")
    set( SRC_FILES ${SRC_FILES} ${SRC_FILES_UNIX} )
endif(WIN32)

if(Ogg_FOUND)
    set(INCLUDE_DIRS    ${INCLUDE_DIRS}
                        ${Ogg_INCLUDE_DIR} )
    set(LINK_DIRS       ${LINK_DIRS}
                        ${Ogg_LIB_DEBUG}
                        ${Ogg_LIB_RELEASE} )
else(Ogg_FOUND)
    set(INCLUDE_DIRS    ${INCLUDE_DIRS}
                        "${RADAKAN_SOURCE_DIR}/external/libogg-1.1.3/include" )
    set(LINK_DIRS       ${LINK_DIRS}
                        ${RADAKAN_BINARY_DIR}/ogg )
endif(Ogg_FOUND)

if(Vorbis_FOUND)
    set(INCLUDE_DIRS    ${INCLUDE_DIRS}
                        ${Vorbis_INCLUDE_DIR} )
    set(LINK_DIRS       ${LINK_DIRS}
                        ${Vorbis_LIB_DEBUG}
                        ${Vorbis_LIB_RELEASE} )
else(Vorbis_FOUND)
    set(INCLUDE_DIRS    ${INCLUDE_DIRS}
                        "${RADAKAN_SOURCE_DIR}/external/libvorbis-1.1.2/include" )
    set(LINK_DIRS       ${LINK_DIRS}
                        ${RADAKAN_BINARY_DIR}/vorbis )
endif(Vorbis_FOUND)

set(INCLUDE_DIRS    ${INCLUDE_DIRS}
                    "${RADAKAN_SOURCE_DIR}/external/audiere/src"
                    "${RADAKAN_SOURCE_DIR}/external/audiere/src/mpaudec")

if(WIN32)
    set(INCLUDE_DIRS    ${INCLUDE_DIRS}
                        ${DirectX_INCLUDE_DIRS})
    set(LINK_DIRS       ${LINK_DIRS}
                        ${DirectX_LIB_DIR})
endif(WIN32)

include_directories( ${INCLUDE_DIRS} )

link_directories( ${LINK_DIRS} )
add_library( audiere SHARED ${SRC_FILES} )

set(DEBUG_LIBS      debug ogg
                    debug vorbis )
set(RELEASE_LIBS    optimized ogg
                    optimized vorbis )

if(WIN32)
    set(DEBUG_LIBS      ${DEBUG_LIBS}
                        debug dsound
                        debug dxguid
                        debug Rpcrt4
                        debug winmm )
    set(RELEASE_LIBS    ${RELEASE_LIBS}
                        optimized dsound
                        optimized dxguid
                        optimized Rpcrt4
                        optimized winmm )
endif(WIN32)
target_link_libraries( audiere  ${RELEASE_LIBS}
                                ${DEBUG_LIBS} )

if(WIN32)
    set_target_properties( audiere PROPERTIES DEFINE_SYMBOL AUDIERE_EXPORTS COMPILE_FLAGS "-DWIN32_LEAN_AND_MEAN -DWIN32 -D_USRDL -DNOMINMAX -DNO_SPEEX -DNO_DUMB -DNO_FLAC -DNO_MP3" )
else(WIN32)
    set_target_properties( audiere PROPERTIES COMPILE_FLAGS "-DNO_SPEEX -DNO_DUMB -DNO_FLAC -DNO_MP3" )
endif(WIN32)

install( TARGETS audiere DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
install( TARGETS audiere DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )
