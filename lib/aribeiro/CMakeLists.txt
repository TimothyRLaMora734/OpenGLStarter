project (aribeiro)

#include_directories( ${GLOBAL_INCLUDE_DIR} )

set( ARIBEIRO_INCLUDE_DIR ${GLOBAL_INCLUDE_DIR})

if(ARIBEIRO_RPI)

    #find_package(EGL REQUIRED)
    #find_package(GLES REQUIRED)
    #include_directories(${EGL_INCLUDE_DIR} ${GLES_INCLUDE_DIR})

        # Workaround: we need to use gles2 on the rpi version
        #get_filename_component(GLES2PATH ${GLES_LIBRARY} PATH)
        #find_library(GLES2_LIBRARY brcmGLESv2 ${GLES2PATH})
        # fallback to old vendor library name
        #if(${GLES2_LIBRARY} STREQUAL "GLES2_LIBRARY-NOTFOUND")
            #find_library(GLES2_LIBRARY GLESv2 ${GLES2PATH})
        #endif()

    #set( ARIBEIRO_LIBRARIES ${EGL_INCLUDE_DIR} ${GLES_INCLUDE_DIR} ${GLES2_LIBRARY})


else()
    set( ARIBEIRO_LIBRARIES ${OPENGL_LIBRARIES})
endif()

if(WIN32)
	#for path operations
	set(ARIBEIRO_LIBRARIES ${ARIBEIRO_LIBRARIES} shlwapi iphlpapi)
	#for multimedia calls
	set(ARIBEIRO_LIBRARIES ${ARIBEIRO_LIBRARIES} winmm)
endif()

#
# inside framework dependencies
#
set(ARIBEIRO_LIBRARIES ${ARIBEIRO_LIBRARIES} glew convertutf png_static zlib )

if (ARIBEIRO_RPI)
	#set(ARIBEIRO_INCLUDE_DIR ${ARIBEIRO_INCLUDE_DIR} /opt/vc/lib/)
	set(ARIBEIRO_LIBRARIES ${ARIBEIRO_LIBRARIES} bcm_host openmaxil vcos vchiq_arm) #pthread
	message (STATUS "All Link Libs: ${ARIBEIRO_LIBRARIES}")
endif()

if (${OS_TARGET} STREQUAL "linux")

set(ARIBEIRO_LIBRARIES ${ARIBEIRO_LIBRARIES} pthread )

endif()


# By default, symbols provided by the sublibs that are not used by mainlib (which is all of them in this case)
# are not used. This changes that.
if (MSVC)
set(ARIBEIRO_LIBRARIES
        -WHOLEARCHIVE:aribeiro
		aribeiro
        ${ARIBEIRO_LIBRARIES})
elseif (APPLE)
set(ARIBEIRO_LIBRARIES
        -Wl,-all_load
		aribeiro
        ${ARIBEIRO_LIBRARIES})
elseif (WIN32)
#MingW...
set(ARIBEIRO_LIBRARIES
		aribeiro
        ${ARIBEIRO_LIBRARIES})
else ()
set(ARIBEIRO_LIBRARIES
        -Wl,--whole-archive
		aribeiro
        -Wl,--no-whole-archive
        ${ARIBEIRO_LIBRARIES})
endif ()

# export global variables
set(ARIBEIRO_INCLUDE_DIR ${ARIBEIRO_INCLUDE_DIR} PARENT_SCOPE)
set(ARIBEIRO_LIBRARIES ${ARIBEIRO_LIBRARIES} PARENT_SCOPE)

FILE( GLOB math_collision_src src/math/collision/*.cpp)
FILE( GLOB math_collision_h src/math/collision/*.h )

FILE( GLOB opengl_src src/opengl/*.cpp)
FILE( GLOB opengl_h src/opengl/*.h )

FILE( GLOB util_src src/util/*.cpp)
FILE( GLOB util_h src/util/*.h )

FILE( GLOB math_src src/math/*.cpp)
FILE( GLOB math_h src/math/*.h )

set( PUBLIC_HEADERS

		src/aribeiro.h
		src/common.h

		${math_h}

		${math_collision_h}

		src/platform/PlatformMutex.h
		src/platform/PlatformThread.h
		src/platform/PlatformSleep.h
		src/platform/PlatformPath.h
		src/platform/PlatformThread.h
		src/platform/PlatformTime.h
		src/platform/PlatformGL.h

        ${util_h}
        src/util/win32/drand48.h


        ${opengl_h}

		src/opengl/shaders/GLShaderColor.h
		src/opengl/shaders/GLShaderTextureColor.h
		src/opengl/shaders/GLShaderFont.h
		src/opengl/shaders/GLShaderYUV2RGB.h
		src/opengl/shaders/GLShaderVertexColor.h
        src/opengl/shaders/GLPostProcessingShader.h

		src/opengl/draw/GLSquare.h

		src/opengl/font/Font.h
		src/opengl/font/GLFont.h
		src/opengl/font/GLStripText.h


		)

set( PRIVATE_HEADERS )

set( SRC

		${math_src}

		${math_collision_src}

		src/platform/PlatformMutex.cpp
		src/platform/PlatformThread.cpp
		src/platform/PlatformSleep.cpp
		src/platform/PlatformPath.cpp
		src/platform/PlatformThread.cpp
		src/platform/PlatformTime.cpp
		src/platform/PlatformGL.cpp

		${util_src}
        src/util/win32/drand48.cpp

        ${opengl_src}

		src/opengl/shaders/GLShaderColor.cpp
		src/opengl/shaders/GLShaderTextureColor.cpp
		src/opengl/shaders/GLShaderFont.cpp
		src/opengl/shaders/GLShaderYUV2RGB.cpp
		src/opengl/shaders/GLShaderVertexColor.cpp

		src/opengl/draw/GLSquare.cpp

		src/opengl/font/Font.cpp
		src/opengl/font/GLFont.cpp
		src/opengl/font/GLStripText.cpp

		)


#Visual Studio filters
#if (MSVC)

	source_group("" FILES
		src/aribeiro.h
		src/common.h
	)

	source_group("math" FILES
		${math_h}
        ${math_src}
	)

	source_group("math\\collision" FILES
		${math_collision_h}
		${math_collision_src}
	)

	source_group("platform" FILES
		src/platform/PlatformMutex.h
		src/platform/PlatformThread.h
		src/platform/PlatformSleep.h
		src/platform/PlatformPath.h
		src/platform/PlatformThread.h
		src/platform/PlatformTime.h
		src/platform/PlatformMutex.cpp
		src/platform/PlatformThread.cpp
		src/platform/PlatformSleep.cpp
		src/platform/PlatformPath.cpp
		src/platform/PlatformThread.cpp
		src/platform/PlatformTime.cpp

		src/platform/PlatformGL.h
		src/platform/PlatformGL.cpp

	)

	source_group("util" FILES
		${util_h}
        ${util_src}
	)

	source_group("util\\win32" FILES
		src/util/win32/drand48.h
		src/util/win32/drand48.cpp
	)

	source_group("opengl" FILES
        ${opengl_h}
        ${opengl_src}
	)

	source_group("opengl\\shaders" FILES

		src/opengl/shaders/GLShaderTextureColor.h
		src/opengl/shaders/GLShaderColor.h
		src/opengl/shaders/GLShaderFont.h
		src/opengl/shaders/GLShaderYUV2RGB.h

		src/opengl/shaders/GLShaderTextureColor.cpp
		src/opengl/shaders/GLShaderColor.cpp
		src/opengl/shaders/GLShaderFont.cpp
		src/opengl/shaders/GLShaderYUV2RGB.cpp

		src/opengl/shaders/GLShaderVertexColor.h
		src/opengl/shaders/GLShaderVertexColor.cpp

        src/opengl/shaders/GLPostProcessingShader.h

	)

	source_group("opengl\\draw" FILES

		src/opengl/draw/GLSquare.h
		src/opengl/draw/GLSquare.cpp

	)

	source_group("opengl\\font" FILES

		src/opengl/font/Font.h
		src/opengl/font/GLFont.h
		src/opengl/font/GLStripText.h

		src/opengl/font/Font.cpp
		src/opengl/font/GLFont.cpp
		src/opengl/font/GLStripText.cpp

	)


#endif()


# By default, symbols provided by the sublibs that are not used by mainlib (which is all of them in this case)
# are not used. This changes that.
#if (WIN32)
#    set_target_properties(${PROJECT_NAME} PROPERTIES
#        LINK_FLAGS "/WHOLEARCHIVE"
#    )
#elseif (APPLE)
#    set_target_properties(${PROJECT_NAME} PROPERTIES
#        LINK_FLAGS "-Wl,-all_load"
#    )
#else ()
#    set_target_properties(${PROJECT_NAME} PROPERTIES
#        LINK_FLAGS "-Wl,--whole-archive"
#    )
#endif ()


add_library( ${PROJECT_NAME} STATIC ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SRC} )

#message(STATUS ${GLOBAL_INCLUDE_DIR}/${PROJECT_NAME}/)

file(COPY ${PUBLIC_HEADERS}
	DESTINATION ${GLOBAL_INCLUDE_DIR}/${PROJECT_NAME}/)

macro(LinkARibeiro)
	target_link_libraries( ${PROJECT_NAME}
		aribeiro
		${ARIBEIRO_LIBRARIES} )
endmacro()
