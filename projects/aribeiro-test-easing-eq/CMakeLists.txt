project (aribeiro-test-easing-eq)

#file(GLOB_RECURSE  gamelogic_src GameLogic/*.cpp GameLogic/*.h )

set( HEADERS
	LineRenderer.h
	EasingEqDrawer.h
	PointRenderer.h
)

set( SRC
	main.cpp
)

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS})

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "TESTS")

target_link_libraries( ${PROJECT_NAME}
	#aribeiro
	#sfml-network sfml-graphics sfml-window sfml-system sfml-audio
	sfml-window sfml-graphics
	${ARIBEIRO_LIBRARIES} )

PostBuildConfig( ${PROJECT_NAME} )
PostBuildCopy( ${PROJECT_NAME} resources )
