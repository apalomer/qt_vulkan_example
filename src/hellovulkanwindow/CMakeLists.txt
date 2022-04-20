add_executable(hello_vulkan_window
  hellovulkanwindow.cpp
  hellovulkanwindow.h
  main.cpp
)
target_link_libraries(hello_vulkan_window Qt5::Core Qt5::Widgets ${Vulkan_LIBS})

install(TARGETS hello_vulkan_window
  EXPORT Qt_Vulkan_Examples
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  COMPONENT dev
)
