# set minimum cmake version and policy
cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0048 NEW)

# project name and language
project(VernierLibrary VERSION 1.1 LANGUAGES CXX)

# build options, for example to skip tests building use $ cmake .. -DBUILD_TESTS=OFF
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_DOCUMENTATION "Build documentation" ON)
option(USE_FFTW "Use FFTW" ON)
option(USE_OPENCV "Use OpenCV" ON)

# set build mode type
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# require C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DOPENCV_DISABLE_EIGEN_TENSOR_SUPPORT)

# Add path to a FindSomePackage.cmake file
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# FFTW usage
if (USE_FFTW)
  if (WIN32) # true when the target system is Windows, including Win64.
	  message(STATUS "Enabling FFTW support.")    
    set(FFTW3_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/3rdparty/fftw3)
    set(FFTW3_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/3rdparty/fftw3)
    set(FFTW3_LIBRARIES ${CMAKE_SOURCE_DIR}/3rdparty/fftw3/libfftw3-3.lib)
	  execute_process(COMMAND ${CMAKE_SOURCE_DIR}/3rdparty/pathed/pathed.exe -a ${CMAKE_SOURCE_DIR}/3rdparty/fftw3)
  else(WIN32)
    message(STATUS "Enabling FFTW support.")
    # find_package(FFTW3 REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(FFTW REQUIRED fftw3 IMPORTED_TARGET)
    include_directories(PkgConfig::FFTW)
    link_libraries(PkgConfig::FFTW)
  endif(WIN32)
else(USE_FFTW)
  message(STATUS "Disabling FFTW support. Using Ooura's fft instead.")
  add_subdirectory(3rdparty/ooura/fft2d)
endif(USE_FFTW)

# OpenCV usage
if (USE_OPENCV)
  if (WIN32) # true when the target system is Windows, including Win64.
    message(STATUS "Enabling OpenCV support.")    
    find_package(OpenCV REQUIRED) 
    execute_process(COMMAND ${CMAKE_SOURCE_DIR}/3rdparty/pathed/pathed.exe -a ${_OpenCV_LIB_PATH})
  else(WIN32)
    message(STATUS "Enabling OpenCV support.")
    find_package(OpenCV REQUIRED)
  endif(WIN32)
else(USE_OPENCV)
  message(STATUS "Disabling OpenCV support.")
endif(USE_OPENCV)

# add library core
add_subdirectory(src)
add_subdirectory(3rdparty/gdstk)

# add examples
if (BUILD_EXAMPLES)
add_subdirectory(examples)
endif()

# unit tests
if (BUILD_TESTS)
  message(STATUS "Generating tests.")
  add_subdirectory(3rdparty/matio)
  add_subdirectory(test)

  file(GLOB TESTS test/*.cpp)

  foreach(file ${TESTS})
    get_filename_component(testName ${file} NAME_WE) 
    add_test(NAME ${testName} COMMAND ${testName} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test)
  endforeach(file)

  enable_testing ()
else()
  message(STATUS "Skiping tests.")
endif()

# documentation generation (doxygen)
if (BUILD_DOCUMENTATION)
  message(STATUS "Generating documentation.")
  add_subdirectory(doc)
else()
  message(STATUS "Skiping documentation.")
endif()
