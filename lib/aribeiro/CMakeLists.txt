project (aribeiro)

#include_directories( ${GLOBAL_INCLUDE_DIR} )

set( ARIBEIRO_INCLUDE_DIR ${GLOBAL_INCLUDE_DIR})
set( ARIBEIRO_LIBRARIES ${OPENGL_LIBRARIES})

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

if (${OS_TARGET} STREQUAL "linux")

set(ARIBEIRO_LIBRARIES ${ARIBEIRO_LIBRARIES} pthread )

endif()

# export global variables
set(ARIBEIRO_INCLUDE_DIR ${ARIBEIRO_INCLUDE_DIR} PARENT_SCOPE)
set(ARIBEIRO_LIBRARIES ${ARIBEIRO_LIBRARIES} PARENT_SCOPE)

FILE( GLOB math_collision_src src/math/collision/*.cpp)
FILE( GLOB math_collision_h src/math/collision/*.h )

set( PUBLIC_HEADERS

		src/aribeiro.h
		src/common.h
		
		#src/gl/glUtil.h
		#src/gl/GLTexture.h
		#src/gl/GLSquare.h
		#src/gl/GLShaderYUV2RGB.h
		#src/gl/font/ConvertUTF.h
		#src/gl/font/Font.h
		
		src/math/all_math.h
		src/math/BresenhamIterator.h
		src/math/constants.h
		#src/math/defs.h
		src/math/EasingEquations.h
		src/math/geometricOperations.h
		src/math/mat4.h
		src/math/quat.h
		src/math/tensor4.h
		src/math/vec2.h
		src/math/vec3.h
		src/math/vec4.h
		
		
		${math_collision_h}
		
		
		#src/network/MulticastUdpSocket.h
		#src/network/NetworkConstants.h
		#src/network/NetworkHelper.h
		#src/network/UDP_DataReceive.h
		#src/network/UDP_DataSend.h
		#src/network/BufferFragment.h
		
		#src/util/Delegate.h
		#src/util/getDirectoryToSaveWin32.h
		#src/util/Object.h
		#src/util/PerformaneCounterWin32.h
		#src/util/Platform.h
		#src/util/Thread.h
		#src/util/Time.h
		
		src/platform/PlatformMutex.h
		src/platform/PlatformThread.h
		src/platform/PlatformSleep.h
		src/platform/PlatformPath.h
		src/platform/PlatformThread.h
		src/platform/PlatformTime.h

		src/platform/PlatformGL.h



		src/util/Delegate.h
		src/util/GetFirstMacAddress.h
		src/util/SetNullAndDelete.h
		src/util/NetworkConstants.h
		src/util/PNGHelper.h
		src/util/TransformStack.h
		src/util/Random.h
		src/util/win32/drand48.h
		src/util/PressReleaseDetector.h

		src/opengl/GLTexture.h
		src/opengl/GLShader.h
        src/opengl/GLFramebufferObject.h
        src/opengl/GLPostProcess.h

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
		#src/gl/font/ConvertUTF.c
		#src/gl/font/Font.cpp
		
		src/math/BresenhamIterator.cpp
		src/math/geometricOperations.cpp
		src/math/mat4.cpp
		src/math/quat.cpp
		src/math/tensor4.cpp
		src/math/vec2.cpp
		src/math/vec3.cpp
		src/math/vec4.cpp
		
		${math_collision_src}
		
		
		#src/network/MulticastUdpSocket.cpp
		#src/network/NetworkHelper.cpp
		#src/network/UDP_DataSend.cpp
		
		#src/util/getDirectoryToSaveWin32.cpp
		#src/util/PlatformWin32.cpp
		#src/util/PlatformLinux.cpp

		#${PlatformFiles}

		#src/util/Platform.h
		#src/util/Thread.cpp
		#src/util/Time.cpp

		src/platform/PlatformMutex.cpp
		src/platform/PlatformThread.cpp
		src/platform/PlatformSleep.cpp
		src/platform/PlatformPath.cpp
		src/platform/PlatformThread.cpp
		src/platform/PlatformTime.cpp
		src/platform/PlatformGL.cpp

		src/util/GetFirstMacAddress.cpp
		src/util/PNGHelper.cpp
		src/util/Random.cpp
		src/util/win32/drand48.cpp
		src/util/PressReleaseDetector.cpp

		src/opengl/GLTexture.cpp
		src/opengl/GLShader.cpp
        src/opengl/GLFramebufferObject.cpp
        src/opengl/GLPostProcess.cpp
		
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
		src/math/all_math.h
		src/math/AABB.h
		src/math/BresenhamIterator.h
		src/math/constants.h
		#src/math/defs.h
		src/math/EasingEquations.h
		src/math/geometricOperations.h
		src/math/mat4.h
		src/math/quat.h
		src/math/tensor4.h
		src/math/vec2.h
		src/math/vec3.h
		src/math/vec4.h
		src/math/AABB.cpp
		src/math/BresenhamIterator.cpp
		src/math/geometricOperations.cpp
		src/math/mat4.cpp
		src/math/quat.cpp
		src/math/tensor4.cpp
		src/math/vec2.cpp
		src/math/vec3.cpp
		src/math/vec4.cpp
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
		src/util/Delegate.h
		src/util/GetFirstMacAddress.h
		src/util/SetNullAndDelete.h
		src/util/GetFirstMacAddress.cpp
		src/util/NetworkConstants.h
		src/util/PNGHelper.h
		src/util/PNGHelper.cpp
		src/util/TransformStack.h
		src/util/Random.h
		src/util/Random.cpp
		src/util/PressReleaseDetector.h
		src/util/PressReleaseDetector.cpp
	)
	
	source_group("util\\win32" FILES
		src/util/win32/drand48.h
		src/util/win32/drand48.cpp
	)

	source_group("opengl" FILES
		src/opengl/GLTexture.h
		src/opengl/GLTexture.cpp
		src/opengl/GLShader.h
		src/opengl/GLShader.cpp
        src/opengl/GLFramebufferObject.h
        src/opengl/GLFramebufferObject.cpp
        src/opengl/GLPostProcess.h
        src/opengl/GLPostProcess.cpp
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

add_library( ${PROJECT_NAME} STATIC ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SRC} )

#message(STATUS ${GLOBAL_INCLUDE_DIR}/${PROJECT_NAME}/)

file(COPY ${PUBLIC_HEADERS}
	DESTINATION ${GLOBAL_INCLUDE_DIR}/${PROJECT_NAME}/)

macro(LinkARibeiro)
	target_link_libraries( ${PROJECT_NAME}
		aribeiro
		${ARIBEIRO_LIBRARIES} )
endmacro()
