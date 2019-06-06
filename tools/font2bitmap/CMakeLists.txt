PROJECT( font2bitmap )

# Getting the source files
FILE( GLOB HEADERS *.h )
FILE( GLOB SRC *.cpp)


#SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)


if(WIN32)
	set( ADITIONAL_SRC win32/getopt.c win32/getopt.h )
	set( ADITIONAL_DIR win32 )
	
	SOURCE_GROUP(win32 FILES win32/getopt.c win32/getopt.h)
ENDIF()

include_directories(${ADITIONAL_DIR})

add_executable(${PROJECT_NAME}
		${HEADERS}
		${SRC}
		${ADITIONAL_SRC}
)

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "TOOLS")

# Setting dependencies
#add_dependencies( ${PROJECT_NAME} aSilva )

# Linking libraries
target_link_libraries( ${PROJECT_NAME} 
	#aribeiro
	${ARIBEIRO_LIBRARIES} 
	freetype )

PostBuildCopyFile( ${PROJECT_NAME} charset.utf8 )
PostBuildCopyFile( ${PROJECT_NAME} mailrays.ttf )
PostBuildCopyFile( ${PROJECT_NAME} CreateFontWin32.bat )
PostBuildCopyFile( ${PROJECT_NAME} goudy_bookletter_1911.otf )



