file(GLOB HEADERS *.hpp)

file(GLOB SOURCES *.cpp)

add_library(ooura STATIC ${SOURCES})

install(TARGETS ooura DESTINATION lib)

install(FILES ${HEADERS} DESTINATION include/${CMAKE_PROJECT_NAME})
