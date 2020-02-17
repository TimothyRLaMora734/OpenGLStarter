project (flv)

FILE( GLOB src *.h *.cpp)

add_executable(${PROJECT_NAME} ${src})

source_group("" FILES ${src})

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "RPI")

target_link_libraries( ${PROJECT_NAME} ${ARIBEIRO_LIBRARIES} libflv )

copy_3rdparty_dll(${PROJECT_NAME})
copy_file_after_build(${PROJECT_NAME} test.h264)
