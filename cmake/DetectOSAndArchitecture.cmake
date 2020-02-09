if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
	set( OS_TARGET android )
	set( ARCH_TARGET ${CMAKE_ANDROID_ARCH_ABI} )
else()
	if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
            set( ARCH_TARGET arm64 )
        else()
            set( ARCH_TARGET x64 )
		endif()
	else()
        if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
            set( ARCH_TARGET arm32 )
        else()
            set( ARCH_TARGET x86 )
		endif()
	endif()
	if(WIN32)
	    set( OS_TARGET win )
	elseif(APPLE)
	    set( OS_TARGET mac )
    else()
	    set( OS_TARGET linux )
	endif()
endif()

message(STATUS "${OS_TARGET} -> ${ARCH_TARGET}" )
