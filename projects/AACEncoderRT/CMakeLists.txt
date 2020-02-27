project (AACEncoderRT)

FILE( GLOB src *.h *.cpp)

add_executable(${PROJECT_NAME} ${src})

source_group("" FILES ${src})

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "RPI")

copy_3rdparty_dll(${PROJECT_NAME})

target_link_libraries(  ${PROJECT_NAME}
                        PUBLIC
                        ${ARIBEIRO_LIBRARIES}
                        rtaudio-wrapper
                        ffmpeg-wrapper
                        libflv
)
