cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(Simulation)

# Find and include Geant4
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# Find the ROOT package
find_package(ROOT REQUIRED COMPONENTS RIO Hist Tree)

# Include ROOT's header files
include_directories(${ROOT_INCLUDE_DIRS})

# Explicitly list source and header files
file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

# Add the executable
add_executable(sim ${sources})

# Link your executable with Geant4 and ROOT libraries
target_link_libraries(sim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

# Create a custom target that depends on sim
add_custom_target(Simulation DEPENDS sim)
