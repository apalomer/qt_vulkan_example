add_executable(hello_vulkan_cubes 
  main.cpp
  camera.cpp
  camera.h
  hellovulkancubes.qrc
  mainwindow.cpp
  mainwindow.h
  mesh.cpp
  mesh.h
  renderer.cpp
  renderer.h
  shader.cpp
  shader.h
  vulkanwindow.cpp
  vulkanwindow.h
)
target_link_libraries(hello_vulkan_cubes Qt5::Core Qt5::Widgets ${Vulkan_LIBS})

install(TARGETS hello_vulkan_cubes
  EXPORT Qt_Vulkan_Examples
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  COMPONENT dev
)
