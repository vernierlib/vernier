# Python bindings for the VERNIER library (nanobind).
#
# Produces the `pyvernier` extension module wrapping the native C++ classes.
# nanobind requires C++17, so this target is compiled as C++17 even though the
# core library targets C++14.

# nanobind needs the Python interpreter and the Development.Module component
# (headers + linking helpers, without embedding the interpreter).
find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development.Module)

# Vendored nanobind (3rdparty/nanobind). A separate binary directory is required
# because the source lives outside the current directory.
add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/nanobind ${CMAKE_BINARY_DIR}/nanobind)

nanobind_add_module(pyvernier vernier_python.cpp)

# nanobind switches the target to C++17 automatically; make it explicit.
set_target_properties(pyvernier PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_include_directories(pyvernier PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/3rdparty
  ${CMAKE_SOURCE_DIR}/3rdparty/gdstk/include
  ${OpenCV_INCLUDE_DIRS})

add_definitions(-DOPENCV_DISABLE_EIGEN_TENSOR_SUPPORT)

target_link_libraries(pyvernier PRIVATE vernier ${OpenCV_LIBS})

find_package(Eigen3 REQUIRED)
target_link_libraries(pyvernier PRIVATE Eigen3::Eigen)

if (USE_FFTW)
  target_include_directories(pyvernier PRIVATE ${FFTW3_INCLUDE_DIRS})
  target_link_libraries(pyvernier PRIVATE ${FFTW3_LIBRARIES})
  target_compile_definitions(pyvernier PRIVATE USE_FFTW)
else()
  target_link_libraries(pyvernier PRIVATE ooura)
endif()

if(NOT WIN32)
  target_link_libraries(pyvernier PRIVATE gdstk)
endif()

# Copy the Python tests and example next to the built module so they can be run
# straight from the build tree (the module lands in this binary directory).
file(GLOB PYTHON_FILES *.py)
foreach(file ${PYTHON_FILES})
  get_filename_component(filename ${file} NAME)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${filename} ${CMAKE_CURRENT_BINARY_DIR}/${filename} COPYONLY)
endforeach(file)
