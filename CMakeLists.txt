cmake_minimum_required(VERSION 3.8)

project( flatbuffers-examples )

add_executable ( main src/main.cpp )

#
# flatbuffers
#
find_path( FLATBUFFERS_INCLUDE_DIR flatbuffers/flatbuffers.h )
find_library( FLATBUFFERS_LIBRARY flatbuffers )
target_include_directories( main PRIVATE ${FLATBUFFERS_INCLUDE_DIR} )
target_link_libraries( main PRIVATE ${FLATBUFFERS_LIBRARY} )

target_include_directories( main PRIVATE ./schema )
