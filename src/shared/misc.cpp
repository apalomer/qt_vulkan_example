#include "misc.h"

#include <QVulkanFunctions>

int selectPhysicalDevice(QVulkanWindow *vulkan_window)
{
  auto gpuCount = uint32_t{ 0 };
  auto q_vulkan_instance = vulkan_window->vulkanInstance();
  auto vulkan_instance = q_vulkan_instance->vkInstance();
  QVulkanFunctions *f = q_vulkan_instance->functions();
  f->vkEnumeratePhysicalDevices(vulkan_instance, &gpuCount, nullptr);
  if (gpuCount == 0)
    qFatal("failed to find GPUs with Vulkan support!");
  auto devices = std::vector<VkPhysicalDevice>(gpuCount);
  f->vkEnumeratePhysicalDevices(vulkan_instance, &gpuCount, devices.data());
  int best(-1);
  int current_index(0);
  auto best_props = VkPhysicalDeviceProperties{};
  for (const auto &device : devices)
  {
    auto props = VkPhysicalDeviceProperties{};
    f->vkGetPhysicalDeviceProperties(device, &props);

    if (current_index == 0)
    {
      best = 0;
      best_props = props;
      ++current_index;
      continue;
    }

    // Determine the type of the physical device
    if (props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
      if (best_props.deviceType != VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
      {
        best_props = props;
        best = current_index;
      }
    }
    else if (props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
    {
      if (best_props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_OTHER ||
          best_props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU ||
          best_props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU)
      {
        best_props = props;
        best = current_index;
      }
    }
    else
      continue;
    qInfo() << "Device number: " << current_index;
    qInfo() << "Device type: "
            << QString::asprintf("Active physical device name: '%s' version %d.%d.%d\nAPI version %d.%d.%d\n",
                                 props.deviceName, VK_VERSION_MAJOR(props.driverVersion),
                                 VK_VERSION_MINOR(props.driverVersion), VK_VERSION_PATCH(props.driverVersion),
                                 VK_VERSION_MAJOR(props.apiVersion), VK_VERSION_MINOR(props.apiVersion),
                                 VK_VERSION_PATCH(props.apiVersion));

    // Determine the available device local memory.
    // auto memoryProps = VkPhysicalDeviceMemoryProperies{};
    // f->vkGetPhysicalDeviceMemoryProperties(device, &memoryProps);
    //
    // auto heapsPointer = memoryProps.memoryHeaps;
    // auto heaps = std::vector<VkMemoryHeap>(heapsPointer, heapsPointer + memoryProps.memoryHeapCount);
    //
    // for (const auto &heap : heaps)
    // {
    //   if (heap.flags & VkMemoryHeapFlagBits::VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
    //   {
    //     // Device local heap, should be size of total GPU VRAM.
    //     // heap.size will be the size of VRAM in bytes. (bigger is better)
    //   }
    // }

    ++current_index;
  }

  if (best < 0)
    qFatal("failed to find GPUs with necessary Vulkan support!");
  qInfo() << "Best device: " << best << " with "
          << QString::asprintf("physical device name: '%s' version %d.%d.%d\nAPI version %d.%d.%d\n",
                               best_props.deviceName, VK_VERSION_MAJOR(best_props.driverVersion),
                               VK_VERSION_MINOR(best_props.driverVersion), VK_VERSION_PATCH(best_props.driverVersion),
                               VK_VERSION_MAJOR(best_props.apiVersion), VK_VERSION_MINOR(best_props.apiVersion),
                               VK_VERSION_PATCH(best_props.apiVersion));
  return best;
}

FPSCounter::FPSCounter()
{
}

double FPSCounter::gotNewFrame()
{
  // Append new stamp
  m_frame_stamps.push_back(std::chrono::high_resolution_clock::now());

  // Remove old stamps
  if (m_frame_stamps.size() > 100)
    m_frame_stamps.pop_front();

  // Exit
  if (m_frame_stamps.size() == 1)
    return 0;
  return static_cast<double>(m_frame_stamps.size() - 1) /
         std::chrono::duration<double>(m_frame_stamps.back() - m_frame_stamps.front()).count();
}
