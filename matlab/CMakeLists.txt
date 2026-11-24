# Matlab bindings for the VERNIER library.
#
# The bindings are plain .m files calling the C API through loadlibrary, so
# there is nothing to compile here: the package, the C API library and its
# header, and the images of the tests are gathered in the binary directory,
# ready to be added to the Matlab path. Matlab is only needed to run the tests.

if(NOT BUILD_C_API)
  message(STATUS "Skiping Matlab bindings (they need the C API, build with -DBUILD_C_API=ON).")
  return()
endif()

message(STATUS "Adding Matlab bindings.")

# the +vernier package, the tests and the example
file(GLOB_RECURSE MATLAB_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.m)
foreach(file ${MATLAB_FILES})
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${file} ${CMAKE_CURRENT_BINARY_DIR}/${file} COPYONLY)
endforeach(file)

# the header loadlibrary parses to know the functions of the library
configure_file(${CMAKE_SOURCE_DIR}/c/vernier.h ${CMAKE_CURRENT_BINARY_DIR}/vernier.h COPYONLY)

# the images the tests and the example read: the bindings expose the detectors
# only, so they cannot render their own
file(GLOB FIXTURES ${CMAKE_SOURCE_DIR}/c/data/*.pgm)
foreach(file ${FIXTURES})
  get_filename_component(filename ${file} NAME)
  configure_file(${file} ${CMAKE_CURRENT_BINARY_DIR}/data/${filename} COPYONLY)
endforeach(file)
configure_file(${CMAKE_SOURCE_DIR}/examples/megarenaPatternImage_12bits_9um.jpg
  ${CMAKE_CURRENT_BINARY_DIR}/megarenaPatternImage_12bits_9um.jpg COPYONLY)

# the library itself, next to the package, under the plain name (without the
# version) that vernier.Library looks for
# (add_custom_command(TARGET) cannot be used: vernier_c is defined elsewhere)
set(MATLAB_LIBRARY_NAME ${CMAKE_SHARED_LIBRARY_PREFIX}vernier_c${CMAKE_SHARED_LIBRARY_SUFFIX})
add_custom_target(vernier_matlab ALL
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:vernier_c> ${CMAKE_CURRENT_BINARY_DIR}/${MATLAB_LIBRARY_NAME}
  DEPENDS vernier_c
  COMMENT "Copying the C API library next to the Matlab package")

if(BUILD_TESTS)
  find_package(Matlab COMPONENTS MAIN_PROGRAM)
  if(Matlab_FOUND)
    add_test(NAME TestMatlabBindings COMMAND ${Matlab_MAIN_PROGRAM} -batch test_vernier
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  else()
    message(STATUS "Skiping the Matlab tests (Matlab not found).")
  endif()
endif()

install(DIRECTORY +vernier DESTINATION matlab)
install(FILES ${CMAKE_SOURCE_DIR}/c/vernier.h DESTINATION matlab)
