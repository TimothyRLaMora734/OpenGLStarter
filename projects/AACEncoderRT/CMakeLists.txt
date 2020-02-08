project (AACEncoderRT)

FILE( GLOB src *.h *.cpp)
#FILE( GLOB src_openmax openmax/*.h openmax/*.cpp)
#FILE( GLOB src_v4l2 v4l2/*.h v4l2/*.cpp)

add_executable(${PROJECT_NAME}
	${src}
	${src_openmax}
	${src_v4l2}
)

source_group("" FILES ${src})
source_group("openmax" FILES ${src_openmax})
source_group("v4l2" FILES ${src_v4l2})

#RPI OMX Compilation Flag
add_definitions(-DOMX_SKIP64BIT)

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "RPI")

#target_link_libraries( ${PROJECT_NAME}
#		#aribeiro
#		${ARIBEIRO_LIBRARIES} )

#message(STATUS "link: " ${ARIBEIRO_LIBRARIES} )
#LinkARibeiro()

#PostBuildConfig( ${PROJECT_NAME} )
#PostBuildCopy( ${PROJECT_NAME} tests )
#PostBuildCopyFile(${PROJECT_NAME} test.h264)

find_package(OpenMP)

if (OPENMP_FOUND)
    add_definitions(-DOPENMP)
    #include_directories("${OPENMP_INCLUDES}")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

    message(STATUS "OMP ON: " ${OPENMP_INCLUDES} ", " ${OPENMP_INCLUDE_DIR})
endif()


#
# Find Package FFMpeg Libs
#


if (${OS_TARGET} STREQUAL "mac")

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

elseif (${OS_TARGET} STREQUAL "linux")

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

elseif (${OS_TARGET} STREQUAL "win")
##
# windows build
##
	set(FFMPEG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lgpl/ffmpeg/include)
	
if( ${ARCH_TARGET} STREQUAL "x86" )
    set(FFMPEG_LIB_DIR ${CMAKE_SOURCE_DIR}/lgpl/ffmpeg/lib_w32)
elseif( ${ARCH_TARGET} STREQUAL "x64" )
	set(FFMPEG_LIB_DIR ${CMAKE_SOURCE_DIR}/lgpl/ffmpeg/lib_w64)
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

# dlls to copy to project

	set( FFMPEG_DLLS ${FFMPEG_LIB_DIR}/avcodec-58.dll
					 ${FFMPEG_LIB_DIR}/avformat-58.dll
					 ${FFMPEG_LIB_DIR}/avutil-56.dll
					 ${FFMPEG_LIB_DIR}/avdevice-58.dll
					 ${FFMPEG_LIB_DIR}/avfilter-7.dll
					 ${FFMPEG_LIB_DIR}/postproc-55.dll
					 ${FFMPEG_LIB_DIR}/swresample-3.dll
					 ${FFMPEG_LIB_DIR}/swscale-5.dll
	) 
	
	message (STATUS ${FFMPEG_DLLS} )

	# disable safe exception handler on the compiler... it turns it on when is a x86 project
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /SAFESEH:NO")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")

endif()


message(${FFMPEG_INCLUDE_DIR})

target_include_directories(
${PROJECT_NAME}
PUBLIC
${FFMPEG_INCLUDE_DIR}
)

#PRIVATE
#${AVCODEC_INCLUDE_DIR}
#${AVFORMAT_INCLUDE_DIR}
#${AVUTIL_INCLUDE_DIR}
#${AVDEVICE_INCLUDE_DIR}



#target_link_directories(${FFMPEG_LIB_DIR})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${AVDEVICE_LIBRARY})

message(${AVCODEC_LIBRARY})

target_link_libraries(${PROJECT_NAME}

PUBLIC
${ARIBEIRO_LIBRARIES}
rtaudio

${AVCODEC_LIBRARY}
${AVFORMAT_LIBRARY}
${AVUTIL_LIBRARY}
#${AVDEVICE_LIBRARY}
#${AVFILTER_LIBRARY}
#${POSTPROC_LIBRARY}
${SWRESAMPLE_LIBRARY}
#${SWSCALE_LIBRARY}

                        )



