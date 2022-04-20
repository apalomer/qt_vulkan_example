add_executable(hello_vulkan_texture
  hellovulkantexture.cpp
  hellovulkantexture.h
  hellovulkantexture.qrc
  main.cpp
)
target_link_libraries(hello_vulkan_texture Qt5::Core Qt5::Widgets ${Vulkan_LIBS})

install(TARGETS hello_vulkan_texture
  EXPORT Qt_Vulkan_Examples
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  COMPONENT dev
)
