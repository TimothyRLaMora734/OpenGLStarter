cmake_minimum_required(VERSION 3.7)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)

project (rtmp C)

#
# copy_dlls_to_build_dir
#
# Command used in Windows to copy 3rdparty dlls to final binary folder
#
macro(copy_dlls_to_build_dir PROJECT_NAME)
    foreach(FILENAME IN ITEMS ${ARGN})
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}")
			get_filename_component(FILENAME_WITHOUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}" NAME)
            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                        ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
                        $<TARGET_FILE_DIR:${PROJECT_NAME}>/${FILENAME_WITHOUT_PATH} )
        else()
            message(FATAL_ERROR "File Does Not Exists: ${FILENAME}")
        endif()
    endforeach()
endmacro()

set( LIBRTMP_STANDALONE OFF )

#
# OpenSSL package
#
if (WIN32)

	if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
		#64bit
		set(OPENSSL_LIB_DIR lib_win64)
	else()
		#32bit
		set(OPENSSL_LIB_DIR lib_win32)
	endif()

	set(OPENSSL_FOUND ON)
	set(OPENSSL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/openssl/include )
	set(OPENSSL_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/openssl/${OPENSSL_LIB_DIR}/libssl.lib
						  ${CMAKE_CURRENT_SOURCE_DIR}/openssl/${OPENSSL_LIB_DIR}/libcrypto.lib )
	
else()
	find_package(OpenSSL)
endif()

if(OPENSSL_FOUND)
    #add_definitions(-DNO_CRYPTO) # crypto is from openssl
    set(LIBRTMP_USE_OPENSSL ON CACHE BOOL "Compile with OpenSSL (do not turn on with GnuTLS at same time)")
    if (LIBRTMP_USE_OPENSSL)
        add_definitions(-DUSE_OPENSSL)
        include_directories(${OPENSSL_INCLUDE_DIR})
    endif()
endif()

#add_definitions(-DUSE_POLARSSL)

#
# GnuTLS package
#
find_package(GnuTLS)
find_package(Nettle)
find_package(Hogweed)
find_package(GMP)
if (GNUTLS_FOUND AND NETTLE_FOUND AND GMP_FOUND AND HOGWEED_FOUND)
    set( LIBRTMP_USE_GNUTLS OFF CACHE BOOL "Compile with GnuTLS (do not turn on with OpenSSL at same time)")
    if (LIBRTMP_USE_GNUTLS)
        add_definitions(-DUSE_GNUTLS)
        include_directories(${GNUTLS_INCLUDE_DIR} ${NETTLE_INCLUDE_DIRS} ${GMP_INCLUDES} )
    endif()
endif()


if (LIBRTMP_USE_GNUTLS AND LIBRTMP_USE_OPENSSL)
    message(FATAL_ERROR "Do not turn on LIBRTMP_USE_GNUTLS and LIBRTMP_USE_OPENSSL at same time.")
elseif (NOT LIBRTMP_USE_GNUTLS AND NOT LIBRTMP_USE_OPENSSL)
    # Standalone build
    set( LIBRTMP_STANDALONE ON )
    add_definitions(-DLIBRTMP_ONLY)
    add_definitions(-DNO_CRYPTO)
    add_definitions(-DNO_SSL)
else()
    #normal build, need zlib

	if (WIN32)
		#
		# zlib source include
		#
		if(NOT TARGET zlib)
			set( ZLIB_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/zlib )
			message(STATUS "[librtmp] will compile zlib at: ${ZLIB_OUTPUT_DIR}" )
			add_subdirectory( ../../lib/zlib ${ZLIB_OUTPUT_DIR} )
		endif()
		
		set(ZLIB_FOUND ON)
		set(ZLIB_INCLUDE_DIRS ../../lib/zlib ${ZLIB_OUTPUT_DIR})
		set(ZLIB_LIBRARIES zlib)
		
	else()
		find_package(ZLIB REQUIRED)
	endif()
    if(ZLIB_FOUND)
        include_directories(${ZLIB_INCLUDE_DIRS})
    endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

set( PUBLIC_HEADERS
        src/rtmp.h
        src/rtmp_sys.h 
        src/handshake.h 
        src/dh.h 
        src/amf.h
        src/log.h
        src/amf.h 
        src/bytes.h
        src/http.h )

set( PRIVATE_HEADERS )

set( SRC src/rtmp.c 
         src/log.c 
         src/amf.c 
         src/hashswf.c 
         src/parseurl.c )

if(MSVC)
    set(SRC ${SRC} src/librtmp.def)
endif()     

add_library( ${PROJECT_NAME} SHARED ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SRC} )

if (LIBRTMP_USE_GNUTLS)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GNUTLS_LIBRARIES} ${NETTLE_LIBRARIES} ${HOGWEED_LIBRARY} ${GMP_LIBRARIES} )
endif()

if (LIBRTMP_USE_OPENSSL)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENSSL_LIBRARIES})
endif()

if(ZLIB_FOUND)
    target_link_libraries( ${PROJECT_NAME} PRIVATE ${ZLIB_LIBRARIES} )
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 winmm gdi32)
	set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME librtmp)
	copy_dlls_to_build_dir(${PROJECT_NAME} openssl/${OPENSSL_LIB_DIR}/libssl-3.dll
										   openssl/${OPENSSL_LIB_DIR}/libcrypto-3.dll )
	
endif()

if (LIBRTMP_USE_OPENSSL)
    set(USE_OPENSSL ON )
else()
    set(USE_OPENSSL OFF )
endif()

if (LIBRTMP_USE_GNUTLS)
    set(USE_GNUTLS ON )
else()
    set(USE_GNUTLS OFF )
endif()

if (ZLIB_FOUND)
    set(USE_ZLIB ON )
else()
    set(USE_ZLIB OFF )
endif()

message(STATUS "[librtmp]")
message(STATUS "      StandAlone " ${LIBRTMP_STANDALONE} )
message(STATUS "      OpenSSL " ${USE_OPENSSL} )
message(STATUS "      GnuTLS " ${USE_GNUTLS} )
message(STATUS "      zlib " ${USE_ZLIB} )

#target_compile_options( ${PROJECT_NAME} PRIVATE -DEXPORT )
target_compile_definitions( ${PROJECT_NAME} PUBLIC -DEXPORT )
#target_include_directories( ${PROJECT_NAME} PRIVATE -DEXPORT )

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "LIBS/librtmp")

# OpenGLStarter Integration
if (OpenGLStarter_Integration)
	copy_headers_to_include_directory(${PROJECT_NAME} ${PUBLIC_HEADERS})
endif()
