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
  
  include_directories (${FFTW3_INCLUDE_DIRS})
  target_link_libraries(${exampleName} ${FFTW3_LIBRARIES})
  
  if(NOT WIN32)
    target_link_libraries(${exampleName} gdstk)
  endif()

  install(PROGRAMS $<TARGET_FILE:${exampleName}> DESTINATION bin
          RENAME ${CMAKE_PROJECT_NAME}-${exampleName}${CMAKE_EXECUTABLE_SUFFIX})
  
endforeach(file)

file(GLOB FILES *.jpg *.png *.json)

foreach(file ${FILES})
  get_filename_component(filename ${file} NAME) 
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${filename} ${CMAKE_CURRENT_BINARY_DIR}/${filename} COPYONLY)
endforeach(file)

# The examples read these from the working directory, so they are installed
# beside the binaries: a packaged example then runs as-is from bin/.
install(FILES ${FILES} DESTINATION bin)
