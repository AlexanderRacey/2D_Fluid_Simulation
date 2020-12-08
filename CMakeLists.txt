cmake_minimum_required(VERSION 3.11)
# Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Fluid_Simulation)

#### Setup Directories ####
#Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

#### Copy Resources ####
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND}
	-E copy_directory
	"${PROJECT_SOURCE_DIR}/res"
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res
)

## Resources 
file(GLOB_RECURSE RESOURCE_FILES "res/*.*")

#### Simulation ####
file(GLOB_RECURSE SOURCES simulation/*.cpp simulation/*.h)
add_executable(SIMULATION ${SOURCES} ${RESOURCE_FILES})
source_group("resources" FILES ${RESOURCE_FILES})
target_include_directories(SIMULATION PRIVATE ${SFML_INCS})
target_link_libraries(SIMULATION sfml-graphics)
set(EXECUTABLES ${EXECUTABLES} SIMULATION)
add_dependencies(SIMULATION copy_resources)

set_target_properties(SIMULATION
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
)
