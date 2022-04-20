add_executable(main main.cpp hellovulkanwindow.cpp hellovulkanwindow.h)
target_link_libraries(main Qt5::Core Qt5::Widgets ${Vulkan_LIBS})