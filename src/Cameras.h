#pragma once

#include <ofMain.h>

class Cameras
{
public:
  Cameras();
  ofCamera camMain;
  void resetMainCamera();
  ofCamera camTop;
};
