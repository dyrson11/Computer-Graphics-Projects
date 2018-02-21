cmake_minimum_required( VERSION 3.0.0 )

project( finalProject )
add_subdirectory( graph )

include_directories(graph/include)

file(GLOB headers
    graph/include/andres/*.hxx
    graph/include/andres/ilp/*.hxx
    graph/include/andres/graph/*.hxx
    graph/include/andres/graph/multicut/*.hxx
    graph/include/andres/graph/multicut-lifted/*.hxx
    graph/include/andres/graph/twocut-lifted/*.hxx
    graph/include/andres/graph/hdf5/*.hxx
    graph/include/andres/graph/detail/*.hxx
)

# FIND LIBRARIES
find_package( GLEW REQUIRED )
find_package( OpenGL REQUIRED )
find_package( GLUT REQUIRED )

set ( CMAKE_CXX_STANDARD 11 )

if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "-lomp ${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


include_directories( headers/ )

add_executable( test.out
                main.cpp
                src/graph.cpp
                src/model.cpp
                src/program.cpp )

target_link_libraries( test.out GL GLU glut GLEW )
