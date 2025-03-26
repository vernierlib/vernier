include_directories (${CMAKE_SOURCE_DIR}/include)
include_directories (${CMAKE_SOURCE_DIR}/3rdparty)
include_directories (${CMAKE_SOURCE_DIR}/3rdparty/matio/src)
include_directories (${CMAKE_SOURCE_DIR}/3rdparty/gdstk/include)

file(GLOB TESTS *.cpp)

foreach(file ${TESTS})

  get_filename_component(testName ${file} NAME_WE) 
  add_executable (${testName} ${file}) 

  target_link_libraries(${testName} vernier)
  target_link_libraries(${testName} matio)
  
  include_directories (${OpenCV_INCLUDE_DIRS})
  add_definitions(-DOPENCV_DISABLE_EIGEN_TENSOR_SUPPORT)
  target_link_libraries(${testName} ${OpenCV_LIBS})
  
  if (USE_FFTW)
    include_directories (${FFTW3_INCLUDE_DIRS})
    target_link_libraries(${testName} ${FFTW3_LIBRARIES})
    target_compile_definitions(${testName} PUBLIC USE_FFTW)
  else()
    target_link_libraries(${testName} ooura)
  endif()
  
  if(NOT WIN32)
    target_link_libraries(${testName} gdstk)
  endif()
  
endforeach(file)

add_subdirectory(data)
