#pragma once

#include <ofMain.h>

constexpr auto N_CAMERAS = 2.0f;

class Viewports
{
public:
  Viewports();
  ofRectangle viewMain;
  ofRectangle viewTop;
};
