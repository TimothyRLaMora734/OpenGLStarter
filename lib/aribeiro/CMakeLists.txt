project (aribeiro)

#
# General Configuration
#

set(ARIBEIRO_LIBRARIES glew convertutf png_static zlib )

# include RPI libs
if (ARIBEIRO_RPI)
	set(ARIBEIRO_LIBRARIES ${GLES_LIBRARY} ${RPI_LIBRARIES} ${ARIBEIRO_LIBRARIES})
else()
	set(ARIBEIRO_LIBRARIES ${OPENGL_LIBRARIES} ${ARIBEIRO_LIBRARIES})
endif()

#
# The default behaviour of the linker is to
# load symbols just when they are needed.
#
# The code below forces the load of all defined symbols
#
if(OS_TARGET STREQUAL win)
	list(APPEND ARIBEIRO_LIBRARIES
				shlwapi iphlpapi #path operations
				winmm ) #multimedia calls
	if (MSVC)
		#Visual Studio
		set(ARIBEIRO_LIBRARIES -WHOLEARCHIVE:aribeiro aribeiro ${ARIBEIRO_LIBRARIES})
	elseif ()
		#MingW...
		set(ARIBEIRO_LIBRARIES aribeiro ${ARIBEIRO_LIBRARIES})
	endif()
elseif(OS_TARGET STREQUAL mac)
	set(ARIBEIRO_LIBRARIES -Wl,-all_load aribeiro ${ARIBEIRO_LIBRARIES})
elseif(OS_TARGET STREQUAL linux)
	list(APPEND ARIBEIRO_LIBRARIES pthread )
	set(ARIBEIRO_LIBRARIES -Wl,--whole-archive aribeiro -Wl,--no-whole-archive ${ARIBEIRO_LIBRARIES})
endif ()

# export global variables
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


add_library( ${PROJECT_NAME} STATIC ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SRC} )

copy_headers_to_include_directory( ${PROJECT_NAME} ${PUBLIC_HEADERS} )
