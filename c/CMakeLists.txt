# C API of the VERNIER library.
#
# Builds `vernier_c`, a shared library exposing the flat C interface declared in
# vernier.h. Being a shared library with a C ABI, it can be loaded by any
# language able to call C, without matching the C++ compiler the library was
# built with.

add_library(vernier_c SHARED vernier_c.cpp)

# the C++ library is static, so linking it into a shared one requires
# position-independent code (see BUILD_C_API in the top-level CMakeLists)
target_link_libraries(vernier_c vernier ${OpenCV_LIBS})
if(NOT WIN32)
  target_link_libraries(vernier_c gdstk)
endif()

target_include_directories(vernier_c PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(vernier_c PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/3rdparty
  ${CMAKE_SOURCE_DIR}/3rdparty/gdstk/include
  ${OpenCV_INCLUDE_DIRS})

# VERNIER_C_BUILD switches the header to exporting the symbols rather than
# importing them, which only matters on Windows
target_compile_definitions(vernier_c PRIVATE
  VERNIER_C_BUILD
  VERNIER_VERSION="${PROJECT_VERSION}"
  OPENCV_DISABLE_EIGEN_TENSOR_SUPPORT)

# Only the functions of vernier.h are exported: everything else is hidden, and
# the symbols of the static libraries linked in (the C++ library, gdstk) are
# kept out of the dynamic symbol table. Otherwise the C++ internals would be
# exported too, ready to be bound to by mistake or to clash with the OpenCV or
# Eigen an application brings along.
set_target_properties(vernier_c PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN ON)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set_target_properties(vernier_c PROPERTIES LINK_FLAGS "-Wl,--exclude-libs,ALL")
endif()

###############################################################################
# tests, compiled as C to check that the header is usable from C
if(BUILD_TESTS)
  enable_language(C)

  add_executable(TestCApi test_vernier_c.c)
  set_target_properties(TestCApi PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
  target_link_libraries(TestCApi vernier_c)
  if(NOT WIN32)
    target_link_libraries(TestCApi m)
  endif()

  # the test reads its images from data/, next to the binary: the C API only
  # exposes the detectors, so the tests cannot render their own images
  file(GLOB FIXTURES ${CMAKE_CURRENT_SOURCE_DIR}/data/*)
  foreach(file ${FIXTURES})
    get_filename_component(filename ${file} NAME)
    configure_file(${file} ${CMAKE_CURRENT_BINARY_DIR}/data/${filename} COPYONLY)
  endforeach(file)

  add_test(NAME TestCApi COMMAND TestCApi WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

install(TARGETS vernier_c DESTINATION lib)
install(FILES vernier.h DESTINATION include/${CMAKE_PROJECT_NAME})
