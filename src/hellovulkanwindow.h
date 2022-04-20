#include <QVulkanWindow>

class VulkanRenderer : public QVulkanWindowRenderer
{
public:
  VulkanRenderer(QVulkanWindow *w);

  void initResources() override;
  void initSwapChainResources() override;
  void releaseSwapChainResources() override;
  void releaseResources() override;

  void startNextFrame() override;

private:
  QVulkanWindow *m_window;
  QVulkanDeviceFunctions *m_devFuncs;
  QColor background_max_color = Qt::green;
  int iteration = 0;
  int max_iterations = 1000;
};

class VulkanWindow : public QVulkanWindow
{
public:
  QVulkanWindowRenderer *createRenderer() override;
};
