###############################################################################
# set minimum cmake version and policy
cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0048 NEW)

# project name and language
project(VernierLibrary VERSION 1.2 LANGUAGES CXX)

# build options, for example to skip tests building use $ cmake .. -DBUILD_TESTS=OFF
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_DOCUMENTATION "Build documentation" ON)
option(USE_FFTW "Use FFTW" ON)

# set build mode type
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# require C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

###############################################################################
# configuration for windows
if(WIN32)

# add FFTW or Ooura
if(USE_FFTW)
  message(STATUS "Enabling FFTW support.")    
  set(FFTW3_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/3rdparty/fftw3)
  set(FFTW3_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/3rdparty/fftw3)
  set(FFTW3_LIBRARIES ${CMAKE_SOURCE_DIR}/3rdparty/fftw3/libfftw3-3.lib)
  execute_process(COMMAND ${CMAKE_SOURCE_DIR}/3rdparty/pathed/pathed.exe -a ${CMAKE_SOURCE_DIR}/3rdparty/fftw3)
else(USE_FFTW)
  message(STATUS "Disabling FFTW support. Using Ooura's fft instead.")
  add_subdirectory(3rdparty/ooura/fft2d)
endif(USE_FFTW)

# find OpenCV, you may need to set OpenCV_DIR variable to the absolute path to the directory containing OpenCVConfig.cmake file
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
#execute_process(COMMAND ${CMAKE_SOURCE_DIR}/3rdparty/pathed/pathed.exe -a ${OpenCV_LIB_PATH})

# find OpenMP (optional)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  message(STATUS "OpenMP_CXX library status:")
  message(STATUS "    version: ${OpenMP_CXX_VERSION}")
  message(STATUS "    include path: ${OpenMP_CXX_INCLUDE_DIRS}")
  message(STATUS "    library path: ${OpenMP_CXX_LIBRARIES}")
  message(STATUS "    libraries: ${OpenMP_CXX_LIB_NAMES}")
endif(OpenMP_CXX_FOUND)

###############################################################################
# configuration for linux/macox
else(WIN32)

# find FFTW or Ooura
if(USE_FFTW)
  message(STATUS "Enabling FFTW support.")
  #find_package(FFTW3 REQUIRED)
  find_package(PkgConfig REQUIRED)
  pkg_search_module(FFTW REQUIRED fftw3 IMPORTED_TARGET)
  include_directories(PkgConfig::FFTW)
  link_libraries(PkgConfig::FFTW)
  message(STATUS "    version: ${FFTW3_VERSION}")
  message(STATUS "    config: ${FFTW3_LIBRARY_DIRS}")
  message(STATUS "    include path: ${FFTW3_INCLUDE_DIRS}")
  message(STATUS "    libraries: ${FFTW3_LIBRARIES}")
else(USE_FFTW)
  message(STATUS "Disabling FFTW support. Using Ooura's fft instead.")
  add_subdirectory(3rdparty/ooura/fft2d)
endif(USE_FFTW)

# find OpenCV
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "    libraries: ${OpenCV_LIBS}")

# find OpenMP (optional)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  message(STATUS "OpenMP_CXX library status:")
  message(STATUS "    version: ${OpenMP_CXX_VERSION}")
  message(STATUS "    include path: ${OpenMP_CXX_INCLUDE_DIRS}")
  message(STATUS "    library path: ${OpenMP_CXX_LIBRARIES}")
  message(STATUS "    libraries: ${OpenMP_CXX_LIB_NAMES}")
endif(OpenMP_CXX_FOUND)

# add GDS Toolkit
add_subdirectory(3rdparty/gdstk)

endif(WIN32)

###############################################################################
# add library core
add_subdirectory(src)

###############################################################################
# add examples
if (BUILD_EXAMPLES)
add_subdirectory(examples)
endif()

###############################################################################
# unit tests
if(BUILD_TESTS)
  message(STATUS "Adding tests.")
  add_subdirectory(3rdparty/matio)
  add_subdirectory(test)

  file(GLOB TESTS test/*.cpp)
  foreach(file ${TESTS})
    get_filename_component(testName ${file} NAME_WE) 
    add_test(NAME ${testName} COMMAND ${testName} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test)
  endforeach(file)

  file(GLOB EXAMPLES examples/*.cpp)
  foreach(file ${EXAMPLES})
    get_filename_component(testName ${file} NAME_WE) 
    add_test(NAME ${testName} COMMAND ${testName} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/examples)
  endforeach(file)

  enable_testing ()
else(BUILD_TESTS)
  message(STATUS "Skiping tests.")
endif(BUILD_TESTS)

###############################################################################
# documentation generation (doxygen)
if (BUILD_DOCUMENTATION)
  message(STATUS "Adding doxygen documentation.")
  add_subdirectory(doc)
else(BUILD_DOCUMENTATION)
  message(STATUS "Skiping documentation.")
endif(BUILD_DOCUMENTATION)
