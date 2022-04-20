# Set up cmake
cmake_minimum_required (VERSION 3.0)
project (qt_vulkan_example CXX)

# Find LIbrary
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Vulkan
find_package(Vulkan ${Vulkan_MIN_VERSION} REQUIRED)
include_directories(${Vulkan_INCLUDE_DIR})

add_subdirectory(src)
