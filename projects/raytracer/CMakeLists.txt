project (raytracer)

FILE( GLOB src *.h *.cpp)

add_executable(${PROJECT_NAME}
	${src}
)

source_group("" FILES ${src})

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "PUC")

target_link_libraries( ${PROJECT_NAME}
		aribeiro
		${ARIBEIRO_LIBRARIES} )

#message(STATUS "link: " ${ARIBEIRO_LIBRARIES} )
#LinkARibeiro()

#PostBuildConfig( ${PROJECT_NAME} )
PostBuildCopy( ${PROJECT_NAME} tests )

