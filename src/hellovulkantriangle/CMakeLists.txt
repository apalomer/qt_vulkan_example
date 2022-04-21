add_executable(hello_vulkan_triangle
  hellovulkantriangle.qrc
  main.cpp
)
target_link_libraries(hello_vulkan_triangle Qt5::Core Qt5::Widgets ${Vulkan_LIBS} qt_hello_vulkan_example_lib)

install(TARGETS hello_vulkan_triangle
  EXPORT Qt_Vulkan_Examples
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  COMPONENT dev
)
