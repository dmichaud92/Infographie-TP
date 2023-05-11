#include "Cameras.h"

Cameras::Cameras()
{
  camMain.setNearClip(1);
  camMain.setFarClip(100000);
  resetMainCamera();

  camTop.enableOrtho();
  camTop.tiltDeg(-90);
  camTop.setNearClip(10);
  camTop.dolly(2000);
  camTop.setFarClip(100000);
}

void Cameras::resetMainCamera()
{
  camMain.setFov(40);
  camMain.setGlobalPosition(0, 400, 400);
  camMain.setOrientation(glm::vec3(-50, 0, 0));
}
