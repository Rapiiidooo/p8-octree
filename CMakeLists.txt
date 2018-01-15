
cmake_minimum_required (VERSION 2.6)

#Declaration du projet
project(octree)

# The version number.
set (LIBRARY_OUTPUT_DIRECTORY build)
# configure a header file to pass some of the CMake settings
# to the source code
configure_file ("${PROJECT_SOURCE_DIR}/head.h" "${PROJECT_BINARY_DIR}/head.h")

#Déclaration de l'exécutable
add_executable(
	octree
	main.c
	ppm.c
	src.c
	temps.c
)
target_link_libraries(octree m)
target_link_libraries(octree GL)
target_link_libraries(octree glut)
target_link_libraries(octree GLU)
