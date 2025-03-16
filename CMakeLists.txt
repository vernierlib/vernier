# set minimum cmake version and policy
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0048 NEW)

# project name and language
project(VernierLibrary VERSION 1.1 LANGUAGES CXX)

# build options, for example to skip tests building use $ cmake .. -DBUILD_TESTS=OFF
option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCUMENTATION "Build documentation" ON)
option(USE_FFTW "Use FFTW" ON)
option(USE_OPENCV "Use OpenCV" ON)

# set build mode type
set(CMAKE_BUILD_TYPE Debug)

# require C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    find_package(FFTW3 REQUIRED)
  endif(WIN32)
else(USE_FFTW)
  message(STATUS "Disabling FFTW support. Using Ooura's fft instead.")
  add_subdirectory(3rdparty/ooura/fft2d)
endif(USE_FFTW)

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
if (WIN32) # true when the target system is Windows, including Win64.
  execute_process(COMMAND ${CMAKE_SOURCE_DIR}/3rdparty/pathed/pathed.exe -a ${OpenCV_LIB_PATH})
endif(WIN32)

# Find OpenMP (optional)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  message(STATUS "OpenMP_CXX library status:")
  message(STATUS "    version: ${OpenMP_CXX_VERSION}")
  message(STATUS "    include path: ${OpenMP_CXX_INCLUDE_DIRS}")
  message(STATUS "    library path: ${OpenMP_CXX_LIBRARIES}")
  message(STATUS "    libraries: ${OpenMP_CXX_LIB_NAMES}")
endif(OpenMP_CXX_FOUND)

# add library core
add_subdirectory(src)
add_subdirectory(3rdparty/gdstk)

# add examples
add_subdirectory(examples)

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
