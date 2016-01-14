cmake_minimum_required(VERSION 2.6)
project(NextGen)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-deprecated-declarations -Wno-unused-local-typedefs")
set(CMAKE_BUILD_TYPE Debug)

# Extend the CMake module path to find the FindSFML.cmake
# file in {project root}/cmake/Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")

# -------------------------------------------

# TinyXML
file(GLOB_RECURSE LIBS libs/tinyxml/*.cpp libs/tinyxml/*.h)
include_directories(${PROJECT_SOURCE_DIR}/libs/)

# More include stuff
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/)

# Boost and Boost.Python
file(GLOB_RECURSE BOOST_LIBS libs/boost/b2stuff/lib/*.a)
set(BOOST_INCLUDE_DIRS libs/boost/b2stuff/include)
include_directories(${BOOST_INCLUDE_DIRS})


# Python
set(Python_ADDITIONAL_VERSIONS 3.4)
#set(Python_ADDITIONAL_VERSIONS 2.7)
#set(PYTHON_INCLUDE_DIRS /usr/include/python2.7/)
#set(PYTHON_LIBRARY /usr/lib/x86_64-linux-gnu/libpython2.7.so.1)
find_package(PythonLibs 3.4 REQUIRED)
#find_package(PythonInterp 2.7)
include_directories(${PYTHON_INCLUDE_DIRS})

# -------------------------------------------
# The source files to compile
file(GLOB_RECURSE SRC src/*.cpp src/*.hpp)

# The program we're compiling with the sources set above
add_executable(Core ${SRC} ${LIBS} ${PROJECT_SOURCE_DIR}/src/Core/core.cxx)
add_executable(Teach ${SRC} ${LIBS} ${PROJECT_SOURCE_DIR}/src/Teach/teach.cxx)
add_executable(MBExec ${SRC} ${LIBS} ${PROJECT_SOURCE_DIR}/src/MBExec/MBExec.cxx)

# TODO make MBExec not require python or boost

target_link_libraries(Core ${PYTHON_LIBRARY} ${Boost_LIBRARIES} ${BOOST_LIBS})
target_link_libraries(Teach ${PYTHON_LIBRARY} ${Boost_LIBRARIES} ${BOOST_LIBS})
target_link_libraries(MBExec ${PYTHON_LIBRARY} ${Boost_LIBRARIES} ${BOOST_LIBS})

