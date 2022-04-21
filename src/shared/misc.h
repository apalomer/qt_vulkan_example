#pragma once

#include <QVulkanWindow>

#include <chrono>
#include <deque>

#include "qthellowvulkanexampleexport.h"

TRIANGLERENDEREREXPORT int selectPhysicalDevice(QVulkanWindow* vulkan_window);

class TRIANGLERENDEREREXPORT FPSCounter
{
  std::deque<std::chrono::high_resolution_clock::time_point> m_frame_stamps;

public:
  FPSCounter();

  double gotNewFrame();
};
