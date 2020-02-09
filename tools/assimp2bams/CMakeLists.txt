PROJECT( assimp2bams )

# Getting the source files
FILE( GLOB sources *.cpp)
FILE( GLOB headers *.h )

add_executable(${PROJECT_NAME} ${sources} ${headers})

source_group("" FILES ${sources} ${headers})

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "TOOLS")

# Linking libraries
target_link_libraries(${PROJECT_NAME}
	${ARIBEIRO_LIBRARIES} 
	assimp	
)

copy_3rdparty_dll(${PROJECT_NAME})
