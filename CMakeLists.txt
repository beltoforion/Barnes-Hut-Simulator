cmake_minimum_required(VERSION 3.8)
project(barnes-hut-simulator CXX)

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

add_executable(barnes-hut-simulator
    main.cpp
    BHTree.cpp
    IIntegrator.cpp
    IModel.cpp
    IntegratorADB2.cpp
    IntegratorADB3.cpp
    IntegratorADB4.cpp
    IntegratorADB5.cpp
    IntegratorADB6.cpp
    IntegratorEuler.cpp
    IntegratorEulerImproved.cpp
    IntegratorHeun.cpp
    IntegratorRK3.cpp
    IntegratorRK4.cpp
    IntegratorRK5.cpp
    IntegratorRKF4.cpp
    ModelNBody.cpp
    NBodyWnd.cpp
    SDLWnd.cpp
    Types.cpp
    Vector.cpp
  )

target_compile_features(barnes-hut-simulator
  PUBLIC
    cxx_std_11
  )

target_compile_options(barnes-hut-simulator
  PUBLIC
    -Wall
    -Werror
  )

target_include_directories(barnes-hut-simulator
  PUBLIC
    ${PROJECT_SOURCE_DIR}
  )

target_link_libraries(barnes-hut-simulator
  PUBLIC
    SDL
    X11
    OpenGL::GL
    OpenGL::GLU
    OpenMP::OpenMP_CXX
  )