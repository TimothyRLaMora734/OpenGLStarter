############################################################################
# Detect SSE2 compatible environment
############################################################################
if( ARCH_TARGET STREQUAL x86 OR ARCH_TARGET STREQUAL x64 )
    set( ARIBEIRO_SSE2 ON CACHE BOOL "Turn on SSE2 optimizations.")
    if (ARIBEIRO_SSE2)
        message(STATUS "**************")
        message(STATUS "* USING SSE2 *")
        message(STATUS "**************")
		if(MSVC)
			message(STATUS "NOTICE: You are building the code for Visual Studio... the containers (vector, set, map, etc...) need to be aligned with ssealign<T,16>")
			add_definitions_global(-DARIBEIRO_SSE2 /arch:SSE2) # /Gz __stdcall
		else()
			add_definitions_global(-DARIBEIRO_SSE2 -mmmx -msse -msse2 -mfpmath=sse -minline-all-stringops -finline-functions)
		endif()
    endif()
endif()
