file(GLOB HEADERS ${CMAKE_SOURCE_DIR}/include/*.hpp)

file(GLOB SOURCES *.cpp)

add_library(vernier STATIC ${SOURCES})



include_directories (${OpenCV_INCLUDE_DIRS})
add_definitions(-DOPENCV_DISABLE_EIGEN_TENSOR_SUPPORT)
target_link_libraries(vernier ${OpenCV_LIBS})

include_directories (${FFTW3_INCLUDE_DIRS})
target_link_libraries(vernier ${FFTW3_LIBRARIES})

if (USE_CUDA)
  file(GLOB CUDA_SOURCES *.cu)
  target_sources(vernier PRIVATE ${CUDA_SOURCES})
  target_include_directories(vernier PUBLIC ${CMAKE_SOURCE_DIR}/include)
  target_link_libraries(vernier CUDA::cufft CUDA::cudart)
  target_compile_definitions(vernier PUBLIC USE_CUDA)
  set_target_properties(vernier PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
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
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/gdstk/include)

install(TARGETS vernier DESTINATION lib)

install(FILES ${HEADERS} DESTINATION include/${CMAKE_PROJECT_NAME})

# Common.hpp includes <rapidjson/document.hpp>, so the bundled copy has to ship
# with the headers or nothing can be compiled against the installed library.
install(DIRECTORY ${CMAKE_SOURCE_DIR}/3rdparty/rapidjson DESTINATION include)


