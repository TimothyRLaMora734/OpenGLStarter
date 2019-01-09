project (convertutf)

set( PUBLIC_HEADERS
		src/ConvertUTF.h )

set( PRIVATE_HEADERS )

set( SRC 
		src/ConvertUTF.c )

add_library( ${PROJECT_NAME} STATIC ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SRC} )

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "LIBS")

file(COPY ${PUBLIC_HEADERS}
	DESTINATION ${CMAKE_HOME_DIRECTORY}/include/${PROJECT_NAME}/)
