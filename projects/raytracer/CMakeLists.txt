project (raytracer)

FILE( GLOB src *.h *.cpp)

add_executable(${PROJECT_NAME}
	${src}
)

source_group("" FILES ${src})

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "PUC")

target_link_libraries( ${PROJECT_NAME}
		#aribeiro
		${ARIBEIRO_LIBRARIES} )

#message(STATUS "link: " ${ARIBEIRO_LIBRARIES} )
#LinkARibeiro()

#PostBuildConfig( ${PROJECT_NAME} )
PostBuildCopy( ${PROJECT_NAME} tests )


find_package(OpenMP)

if (OPENMP_FOUND)
    add_definitions(-DOPENMP)
    #include_directories("${OPENMP_INCLUDES}")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

    message(STATUS "OMP ON: " ${OPENMP_INCLUDES} ", " ${OPENMP_INCLUDE_DIR})
endif()


