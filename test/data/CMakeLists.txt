file(GLOB FILES *.mat *.jpg *.png *.json)

foreach(file ${FILES})
  get_filename_component(filename ${file} NAME) 
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${filename} ${CMAKE_CURRENT_BINARY_DIR}/${filename} COPYONLY)
endforeach(file)

add_subdirectory(QRCode)
add_subdirectory(stamp)