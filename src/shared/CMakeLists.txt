add_library(triangle_renderer SHARED
  trianglerenderer.cpp
  trianglerenderer.h
)
target_link_libraries(triangle_renderer Qt5::Core Qt5::Widgets ${Vulkan_LIBS})

install(TARGETS triangle_renderer
  EXPORT Qt_Vulkan_Examples
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  COMPONENT dev
)
