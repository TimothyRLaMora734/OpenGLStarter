cmake_minimum_required (VERSION 3.7)

# "For this is how God loved the world:
# he gave his only Son, so that everyone
# who believes in him may not perish
# but may have eternal life."
#
# John 3:16

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

ENABLE_LANGUAGE(CXX)


#
# Detect architecture and target os
#
if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
	set( OS_TARGET android )
	set( ARCH_TARGET ${CMAKE_ANDROID_ARCH_ABI} )
else()
	if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
		set( ARCH_TARGET x64 )
	else()
		set( ARCH_TARGET x86 )
	endif()
	if(WIN32)
	  set( OS_TARGET win )

	  if(CMAKE_COMPILER_IS_GNUCXX)
	#  	set(${PROJECT_NAME}_64bits OFF CACHE BOOL "64 bits compiling")
		#if (${PROJECT_NAME}_64bits)
	#		add_definitions ( -m64 )
	#		set( ARCH_TARGET x64 )
	#		message(STATUS "Force 64 bits compiling...")
	#	else()
	#		add_definitions ( -m32 )
	#		set( ARCH_TARGET x86 )
	#		message(STATUS "Force 32 bits compiling...")
	#	endif()
	  elseif(MSVC)
	  foreach(flag
				CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
				CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO

				CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
				CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
				)
			if(${flag} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
				SET ( ${flag} "${${flag}}" CACHE STRING "" FORCE)
			endif()
		endforeach()
	  endif()
	elseif(APPLE)
	  set( OS_TARGET mac )
	else()
	  set( OS_TARGET linux )

      if( NOT CMAKE_BUILD_TYPE )
        set( CMAKE_BUILD_TYPE Debug CACHE STRING
             "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
             FORCE )
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS None Debug Release RelWithDebInfo MinSizeRel)
      endif()

	endif()
endif()
#
# Global settings
#

message( STATUS "TARGET SYSTEM: " ${OS_TARGET}/${ARCH_TARGET}  )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenGL REQUIRED)
set( GLOBAL_INCLUDE_DIR
	${CMAKE_HOME_DIRECTORY}/include
)
message( STATUS "Global include dirs: " ${GLOBAL_INCLUDE_DIR} )
include_directories( ${GLOBAL_INCLUDE_DIR} 	${OPENGL_INCLUDE_DIR})

#
# global definitions
#
if (${OS_TARGET} STREQUAL "android")
	add_definitions(-fexceptions)
	#add_definitions(-std=c++11)
	#add_definitions(-Ofast)
	#add_definitions(-O)
	#add_definitions(-O2)
	add_definitions(-O3)
	add_definitions(-ftree-vectorize) #vectorize to simd for when detected...
	add_definitions(-Wall)
	add_definitions(-Wno-parentheses)
	add_definitions(-ffast-math)
	if (${ARCH_TARGET} STREQUAL "armeabi-v7a")
		add_definitions(-march=armv7-a)
		add_definitions(-mfpu=neon)
		add_definitions(-mfloat-abi=softfp)
		add_definitions(-ftree-vectorize)
	endif()
elseif(CMAKE_COMPILER_IS_GNUCXX)
	#add_definitions(-O2)
elseif(MSVC)
endif()
# Unicode Font issues
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)
add_definitions(-DSFML_STATIC)
add_definitions(-DGLEW_STATIC)

add_definitions(-DOS_TARGET_${OS_TARGET})
add_definitions(-DARCH_TARGET_${ARCH_TARGET})

if (${OS_TARGET} STREQUAL "mac")

	#set(ARIBEIRO_LIBRARIES ${ARIBEIRO_LIBRARIES} pthread )
	#add_definitions( -all_load )

	#SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -all_load")
	#SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -all_load")

	SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -ObjC")

	#flag to fix static linking on macOS
	#add_definitions(-ObjC)

endif()

#
# PostBuildCopy DLL on windows
#
macro(PostBuildConfig PROJECT_NAME)
	if(WIN32)

		if ( NOT ${OPENAL_LIBRARY} STREQUAL "" )

			#
			# will know the OPENAL_LIBRARY only after sfml compilation...
			#
			#STRING(REGEX REPLACE "[\.]lib" ".dll" OPENAL_LIBRARY_DLL ${OPENAL_LIBRARY})
			#STRING(REGEX REPLACE "libs-msvc-universal" "bin" OPENAL_LIBRARY_DLL ${OPENAL_LIBRARY_DLL})

			set( OPENAL_LIBRARY_DLL ${CMAKE_HOME_DIRECTORY}/lib/sfml/extlibs/bin/${ARCH_TARGET}/openal32.dll )

			message(STATUS "[" ${PROJECT_NAME}  "] Will copy " ${OPENAL_LIBRARY_DLL} " To build.")

			add_custom_command(
				TARGET ${PROJECT_NAME} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy
						${OPENAL_LIBRARY_DLL}
						 $<TARGET_FILE_DIR:${PROJECT_NAME}>/openal32.dll )

		else()

			message(STATUS "[" ${PROJECT_NAME}  "] To copy OpenAL library, you need to include sfml.")

		endif()

#		message(STATUS "steam library file: " ${STEAM_DLL})
#		add_custom_command(
#			TARGET ${PROJECT_NAME} POST_BUILD
#			COMMAND ${CMAKE_COMMAND} -E copy
#					${STEAM_DLL}
#					 $<TARGET_FILE_DIR:${PROJECT_NAME}>/steam_api.dll )

	endif()
endmacro()

macro(PostBuildCopy PROJECT_NAME DIRECTORY)
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
				${CMAKE_CURRENT_SOURCE_DIR}/${DIRECTORY}
				 $<TARGET_FILE_DIR:${PROJECT_NAME}>/${DIRECTORY} )

	message(STATUS "[" ${PROJECT_NAME}  "] Will copy " ${CMAKE_CURRENT_SOURCE_DIR}/${DIRECTORY} " To build.")
endmacro()

macro(PostBuildCopyAlessandroRibeiroContent PROJECT_NAME DIRECTORY)
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
				${CMAKE_HOME_DIRECTORY}/AlessandroRibeiro
				 $<TARGET_FILE_DIR:${PROJECT_NAME}>/${DIRECTORY}/AlessandroRibeiro )

	message(STATUS "[" ${PROJECT_NAME}  "] Will copy " ${CMAKE_HOME_DIRECTORY}/AlessandroRibeiro " To build.")
endmacro()

macro(PostBuildCopyFile PROJECT_NAME FILENAME)
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
				${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
				 $<TARGET_FILE_DIR:${PROJECT_NAME}>/${FILENAME} )

	message(STATUS "[" ${PROJECT_NAME}  "] Will copy " ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME} " To build.")
endmacro()


#
# 3rdParty libraries
#
add_subdirectory( lib/assimp )
add_subdirectory( lib/convertutf )
add_subdirectory( lib/freetype )
add_subdirectory( lib/glew )
add_subdirectory( lib/libpng )
#add_subdirectory( lib/libxml )
#add_subdirectory( lib/ogg )
add_subdirectory( lib/sfml )
add_subdirectory( lib/zlib )

#
# MainLibrary
#
add_subdirectory( lib/aribeiro )

#
# Projects
#
add_subdirectory( projects/aribeiro-test )
add_subdirectory( projects/aribeiro-test-easing-eq )
add_subdirectory( projects/template )
add_subdirectory( projects/game-invader )
add_subdirectory( projects/game-tetris )
add_subdirectory( projects/shader-examples )
add_subdirectory( projects/post-processing )
add_subdirectory( projects/post-processing-power-of-two )

#
# Tools
#
add_subdirectory( tools/font2bitmap )
add_subdirectory( tools/conestep-generator )
add_subdirectory( tools/assimp2bams )

#add_subdirectory( projects/crazytwirl )


#add_subdirectory( src/sfml  ${CMAKE_BINARY_DIR}/bin EXCLUDE_FROM_ALL )
#add_subdirectory( src/steamworks )
#add_subdirectory( src/aribeiro )
#add_subdirectory( src/crazytwirl )

#add_subdirectory( src/aribeiro-test )

