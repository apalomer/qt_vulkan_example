add_executable(hello_vulkan_widget
  hellovulkanwidget.cpp
  hellovulkanwidget.h
  hellovulkanwidget.qrc
  main.cpp
)
target_link_libraries(hello_vulkan_widget Qt5::Core Qt5::Widgets ${Vulkan_LIBS} triangle_renderer)

install(TARGETS hello_vulkan_widget
  EXPORT Qt_Vulkan_Examples
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  COMPONENT dev
)
