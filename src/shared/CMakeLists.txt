add_library(qt_hello_vulkan_example_lib SHARED
  qthellowvulkanexampleexport.h
  trianglerenderer.cpp
  trianglerenderer.h
  misc.h
  misc.cpp
)
target_link_libraries(qt_hello_vulkan_example_lib Qt5::Core Qt5::Widgets ${Vulkan_LIBS})
target_compile_definitions(qt_hello_vulkan_example_lib PRIVATE QTHELLOVULKANEXAMPLELIB)

install(TARGETS qt_hello_vulkan_example_lib
  EXPORT Qt_Vulkan_Examples
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  COMPONENT dev
)
