PROJECT( collision )

# Getting the source files
FILE( GLOB sources *.cpp)
FILE( GLOB headers *.h )

add_executable(${PROJECT_NAME}
		${sources}
		${headers}
)

source_group("" FILES ${sources} ${headers})

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "TESTS")

# Setting dependencies
# add_dependencies( ${PROJECT_NAME} aSilva assimp )

# Linking libraries
target_link_libraries(${PROJECT_NAME} 
	aribeiro
	${ARIBEIRO_LIBRARIES}
	sfml-window sfml-graphics
	
)

#PostBuildCopyFile( ${PROJECT_NAME} goudy_bookletter_1911.otf )


