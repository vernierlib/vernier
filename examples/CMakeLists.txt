include_directories (${CMAKE_SOURCE_DIR}/include)
include_directories (${CMAKE_SOURCE_DIR}/3rdparty)
include_directories (${CMAKE_SOURCE_DIR}/3rdparty/gdstk/include)

file(GLOB EXAMPLES *.cpp)

foreach(file ${EXAMPLES})

  get_filename_component(exampleName ${file} NAME_WE) 
  add_executable (${exampleName} ${file})

  target_link_libraries(${exampleName} vernier)

  include_directories (${OpenCV_INCLUDE_DIRS})
  add_definitions(-DOPENCV_DISABLE_EIGEN_TENSOR_SUPPORT)
  target_link_libraries(${exampleName} ${OpenCV_LIBS})
  
  if (USE_FFTW)
    include_directories (${FFTW3_INCLUDE_DIRS})
    target_link_libraries(${exampleName} ${FFTW3_LIBRARIES})
    target_compile_definitions(${exampleName} PUBLIC USE_FFTW)
  else()
    target_link_libraries(${exampleName} ooura)
  endif()
  
  if(NOT WIN32)
    target_link_libraries(${exampleName} gdstk)
  endif()

  install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${exampleName} DESTINATION bin RENAME ${CMAKE_PROJECT_NAME}-${exampleName})
  
endforeach(file)

file(GLOB FILES *.jpg *.png *.json)

foreach(file ${FILES})
  get_filename_component(filename ${file} NAME) 
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${filename} ${CMAKE_CURRENT_BINARY_DIR}/${filename} COPYONLY)
endforeach(file)
