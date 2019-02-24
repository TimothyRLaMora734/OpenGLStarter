PROJECT( puc2019 )

# Getting the source files
FILE( GLOB src *.h *.cpp)
FILE( GLOB src_util util/*.h util/*.cpp)

add_executable(${PROJECT_NAME}
	${src}
	${src_util}
)

source_group("" FILES ${src})
source_group("util" FILES ${src_util})

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "PUC")

# Linking libraries
target_link_libraries(${PROJECT_NAME} 
	aribeiro
	${ARIBEIRO_LIBRARIES}
	sfml-window sfml-graphics	
)
