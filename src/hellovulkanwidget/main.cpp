/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QApplication>
#include <QLibraryInfo>
#include <QLoggingCategory>
#include <QPlainTextEdit>
#include <QPointer>
#include <QVulkanInstance>
#include <QVulkanFunctions>

#include "hellovulkanwidget.h"

Q_LOGGING_CATEGORY(lcVk, "qt.vulkan")

static QPointer<QPlainTextEdit> messageLogWidget;
static QtMessageHandler oldMessageHandler = nullptr;

static void messageHandler(QtMsgType msgType, const QMessageLogContext &logContext, const QString &text)
{
  if (!messageLogWidget.isNull())
    messageLogWidget->appendPlainText(text);
  if (oldMessageHandler)
    oldMessageHandler(msgType, logContext, text);
}

static int selectPhysicalDevice(QVulkanWindow* vulkan_window)
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

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  messageLogWidget = new QPlainTextEdit(QLatin1String(QLibraryInfo::build()) + QLatin1Char('\n'));
  messageLogWidget->setReadOnly(true);

  oldMessageHandler = qInstallMessageHandler(messageHandler);

  QLoggingCategory::setFilterRules(QStringLiteral("qt.vulkan=true"));

  QVulkanInstance inst;

#ifndef Q_OS_ANDROID
  inst.setLayers(QByteArrayList() << "VK_LAYER_LUNARG_standard_validation");
#else
  inst.setLayers(QByteArrayList() << "VK_LAYER_GOOGLE_threading"
                                  << "VK_LAYER_LUNARG_parameter_validation"
                                  << "VK_LAYER_LUNARG_object_tracker"
                                  << "VK_LAYER_LUNARG_core_validation"
                                  << "VK_LAYER_LUNARG_image"
                                  << "VK_LAYER_LUNARG_swapchain"
                                  << "VK_LAYER_GOOGLE_unique_objects");
#endif

  if (!inst.create())
    qFatal("Failed to create Vulkan instance: %d", inst.errorCode());

  // Instantiate the vulkan window
  VulkanWindow *vulkan_window = new VulkanWindow;
  vulkan_window->setVulkanInstance(&inst);

  // Select physical device
  vulkan_window->setPhysicalDeviceIndex(selectPhysicalDevice(vulkan_window));

  MainWindow mainWindow(vulkan_window, messageLogWidget.data());
  QObject::connect(vulkan_window, &VulkanWindow::vulkanInfoReceived, &mainWindow, &MainWindow::onVulkanInfoReceived);
  QObject::connect(vulkan_window, &VulkanWindow::frameQueued, &mainWindow, &MainWindow::onFrameQueued);

  mainWindow.resize(1024, 768);
  mainWindow.show();

  return app.exec();
}
