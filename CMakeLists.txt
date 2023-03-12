cmake_minimum_required(VERSION 3.8)
project(barnes-hut-simulator)
set(CMAKE_CXX_STANDARD 17)

# Need to install the following libraries in the case of Ubuntu:
#  - libsdl1.2-dev
#  - libsdl-ttf2.0-dev
#  - libsdl-gfx1.2-dev
find_package(SDL REQUIRED)
add_library(SDL INTERFACE IMPORTED)
target_include_directories(SDL INTERFACE ${SDL_INCLUDE_DIR})
target_link_libraries(SDL INTERFACE ${SDL_LIBRARY})

find_package(X11 REQUIRED)
add_library(X11 INTERFACE IMPORTED)
target_include_directories(X11 INTERFACE ${X11_INCLUDE_DIR})
target_link_libraries(X11 INTERFACE ${X11_LIBRARIES})

# Please set "OpenGL_GL_PREFERENCE" to "LEGACY" to use a legacy GL library (libGL.so)
# instead of GLVND libraries (libOpenGL.so, libGSL.so).
#
# Example:
#   cmake .. -DOpenGL_GL_PREFERENCE=LEGACY
#
# Need to install the following libraries in the case of Ubuntu:
#   - libglu1-mesa-dev
#   - mesa-common-dev
#   - freeglut3-dev
find_package(OpenGL REQUIRED)

# Need to install the following libraries in the case of Ubuntu:
#   - libgomp1
find_package(OpenMP REQUIRED)

# Add the include directory
include_directories(include)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ffast-math -fomit-framepointers")

# Set the source files
set(SOURCES
    src/main.cpp
    src/BHTree.cpp
    src/IIntegrator.cpp
    src/IntegratorADB5.cpp
    src/IntegratorADB6.cpp
    src/IntegratorRK4.cpp
    src/IntegratorRK5.cpp
    src/IntegratorRKF4.cpp
    src/ModelNBody.cpp
    src/NBodyWnd.cpp
    src/SDLWnd.cpp
    src/Types.cpp
    src/Vector.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

#target_include_directories(barnes-hut-simulator
#  PUBLIC
#    ${PROJECT_SOURCE_DIR}
#  )

target_link_libraries(barnes-hut-simulator
  PUBLIC
    SDL
    X11
    OpenGL::GL
    OpenGL::GLU
    OpenMP::OpenMP_CXX
  )
