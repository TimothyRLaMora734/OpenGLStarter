############################################################################
# Detect FFmpeg libs
############################################################################
if (OS_TARGET STREQUAL mac)

    set(FFMPEG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lgpl/ffmpeg/include)
    set(FFMPEG_LIB_DIR ${CMAKE_SOURCE_DIR}/lgpl/ffmpeg/lib)

    set( AVCODEC_LIBRARY ${FFMPEG_LIB_DIR}/libavcodec.58.dylib )
    set( AVFORMAT_LIBRARY ${FFMPEG_LIB_DIR}/libavformat.58.dylib )
    set( AVUTIL_LIBRARY ${FFMPEG_LIB_DIR}/libavutil.56.dylib )
    set( AVDEVICE_LIBRARY ${FFMPEG_LIB_DIR}/libavdevice.58.dylib )

    set( AVFILTER_LIBRARY ${FFMPEG_LIB_DIR}/libavfilter.7.dylib )
    set( POSTPROC_LIBRARY ${FFMPEG_LIB_DIR}/libpostproc.55.dylib )
    set( SWRESAMPLE_LIBRARY ${FFMPEG_LIB_DIR}/libswresample.3.dylib )
    set( SWSCALE_LIBRARY ${FFMPEG_LIB_DIR}/libswscale.5.dylib )

    include_directories_global(${FFMPEG_INCLUDE_DIR})

elseif (OS_TARGET STREQUAL linux)

    find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
    find_library(AVCODEC_LIBRARY avcodec)

    find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
    find_library(AVFORMAT_LIBRARY avformat)

    find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
    find_library(AVUTIL_LIBRARY avutil)

    find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)
    find_library(AVDEVICE_LIBRARY avdevice)

    find_path(AVFILTER_INCLUDE_DIR libavfilter/avfilter.h)
    find_library(AVFILTER_LIBRARY avfilter)

    find_path(POSTPROC_INCLUDE_DIR libpostproc/postprocess.h)
    find_library(POSTPROC_LIBRARY postproc)

    find_path(SWRESAMPLE_INCLUDE_DIR libswresample/swresample.h)
    find_library(SWRESAMPLE_LIBRARY swresample)

    find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h)
    find_library(SWSCALE_LIBRARY swscale)

    set(FFMPEG_INCLUDE_DIR
            ${AVCODEC_INCLUDE_DIR}
            ${AVFORMAT_INCLUDE_DIR}
            ${AVUTIL_INCLUDE_DIR}
            ${AVDEVICE_INCLUDE_DIR}
            ${AVFILTER_INCLUDE_DIR}
            ${POSTPROC_INCLUDE_DIR}
            ${SWRESAMPLE_INCLUDE_DIR}
            ${SWSCALE_INCLUDE_DIR} )

    list(REMOVE_DUPLICATES FFMPEG_INCLUDE_DIR)

    if ("${FFMPEG_INCLUDE_DIR}" MATCHES "(NOTFOUND)" )
        message(FATAL_ERROR "no FFmpeg dev libraries found")
    endif()

    include_directories_global(${FFMPEG_INCLUDE_DIR})

elseif (OS_TARGET STREQUAL win)
    ##
    # windows build
    ##
    set(FFMPEG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lgpl/ffmpeg/include)

    if( ARCH_TARGET STREQUAL x86 )
        set(FFMPEG_LIB_DIR lgpl/ffmpeg/lib_w32)
    elseif( ARCH_TARGET STREQUAL x64 )
        set(FFMPEG_LIB_DIR lgpl/ffmpeg/lib_w64)
    else()
        message(FATAL_ERROR "windows FFmpeg build arch not supported...")
    endif()

    set( AVCODEC_LIBRARY ${FFMPEG_LIB_DIR}/avcodec.lib )
    set( AVFORMAT_LIBRARY ${FFMPEG_LIB_DIR}/avformat.lib )
    set( AVUTIL_LIBRARY ${FFMPEG_LIB_DIR}/avutil.lib )
    set( AVDEVICE_LIBRARY ${FFMPEG_LIB_DIR}/avdevice.lib )

    set( AVFILTER_LIBRARY ${FFMPEG_LIB_DIR}/avfilter.lib )
    set( POSTPROC_LIBRARY ${FFMPEG_LIB_DIR}/postproc.lib )
    set( SWRESAMPLE_LIBRARY ${FFMPEG_LIB_DIR}/swresample.lib )
    set( SWSCALE_LIBRARY ${FFMPEG_LIB_DIR}/swscale.lib )

    # dlls to copy to projects
    register_dll( ${FFMPEG_LIB_DIR}/avcodec-58.dll
                  ${FFMPEG_LIB_DIR}/avformat-58.dll
                  ${FFMPEG_LIB_DIR}/avutil-56.dll
                  ${FFMPEG_LIB_DIR}/avdevice-58.dll
                  ${FFMPEG_LIB_DIR}/avfilter-7.dll
                  ${FFMPEG_LIB_DIR}/postproc-55.dll
                  ${FFMPEG_LIB_DIR}/swresample-3.dll
                  ${FFMPEG_LIB_DIR}/swscale-5.dll )

    if (MSVC AND ARCH_TARGET STREQUAL x86)
        # disable safe exception handler on the compiler... 
        # it is for x86 arch on VS
        add_linker_flags_global(/SAFESEH:NO)
    endif()

    include_directories_global(${FFMPEG_INCLUDE_DIR})

else()

    message(FATAL_ERROR "FFmpeg not configured for this OS")

endif()
message(STATUS "[FFmpeg Detector] Defined Libraries Variables: " )
message(STATUS "    AVCODEC_LIBRARY")
message(STATUS "    AVFORMAT_LIBRARY") 
message(STATUS "    AVUTIL_LIBRARY")
message(STATUS "    AVDEVICE_LIBRARY") 
message(STATUS "    AVFILTER_LIBRARY") 
message(STATUS "    POSTPROC_LIBRARY")
message(STATUS "    SWRESAMPLE_LIBRARY") 
message(STATUS "    SWSCALE_LIBRARY")
