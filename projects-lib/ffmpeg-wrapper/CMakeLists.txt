project (ffmpeg-wrapper)

FILE( GLOB PUBLIC_HEADERS *.h)
FILE( GLOB SRC *.cpp)

add_library( ${PROJECT_NAME} STATIC ${PUBLIC_HEADERS} ${SRC} )

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "PROJECTLIBS")

#ffmpeg link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC 
    ${AVCODEC_LIBRARY}
    ${AVFORMAT_LIBRARY}
    ${AVUTIL_LIBRARY}
    #${AVDEVICE_LIBRARY}
    #${AVFILTER_LIBRARY}
    #${POSTPROC_LIBRARY}
    ${SWRESAMPLE_LIBRARY}
    #${SWSCALE_LIBRARY}
)

if (OpenGLStarter_Integration)
	copy_headers_to_include_directory(${PROJECT_NAME} ${PUBLIC_HEADERS})
endif()
