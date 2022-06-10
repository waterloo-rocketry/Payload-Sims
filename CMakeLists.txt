# Setup the project
cmake_minimum_required(VERSION 2.9 FATAL_ERROR)
project(BuildTest)

# Find Geant4 package, activate UI and vis drivers
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

# Setup G4 directories and compile definitions
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Locate sources and headers
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Add executable and link to G4 libraries
add_executable(BuildTest BuildTest.cpp ${sources} ${headers})
target_link_libraries(BuildTest ${Geant4_LIBRARIES})

# Install executable to 'bin' directory under CMAKE_INSTALL_PREFIX
install(TARGETS BuildTest DESTINATION bin)

add_custom_command(TARGET BuildTest POST_BUILD COMMAND cp ${CMAKE_SOURCE_DIR}/macros/*.mac ${CMAKE_BINARY_DIR})
