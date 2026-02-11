file(GLOB HEADERS *.hpp)

file(GLOB SOURCES *.cpp)

add_library(vernier STATIC ${SOURCES})



include_directories (${OpenCV_INCLUDE_DIRS})
add_definitions(-DOPENCV_DISABLE_EIGEN_TENSOR_SUPPORT)
target_link_libraries(vernier ${OpenCV_LIBS})

if (USE_FFTW)
  include_directories (${FFTW3_INCLUDE_DIRS})
  target_link_libraries(vernier ${FFTW3_LIBRARIES})
  target_compile_definitions(vernier PUBLIC USE_FFTW)
else()
  target_link_libraries(vernier ooura)
endif()

if (APPLE)
  target_link_directories(vernier PUBLIC /usr/local/lib)
endif()

if(OpenMP_CXX_FOUND)
  target_link_libraries(vernier OpenMP::OpenMP_CXX)
endif()

if(NOT WIN32)
#  include_directories(${EIGEN3_INCLUDE_DIR})
  target_link_libraries(vernier gdstk)
  target_compile_options(vernier PRIVATE -march=native)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/gdstk/include)

install(TARGETS vernier DESTINATION lib)

install(FILES ${HEADERS} DESTINATION include/${CMAKE_PROJECT_NAME})


