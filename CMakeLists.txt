# CMakeList.txt : CMake project for hamiltonian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.13)

project ("hamiltonian")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

configure_file (
    "${PROJECT_SOURCE_DIR}/src/manager.py"
    "${PROJECT_BINARY_DIR}/manager.py"
	COPYONLY
)
configure_file (
    "${PROJECT_SOURCE_DIR}/src/batch.sh"
    "${PROJECT_BINARY_DIR}/batch.sh"
	COPYONLY
)

add_executable (hamiltonian
	"src/hamiltonian.cpp"
	"src/hamiltonian.h"
)

