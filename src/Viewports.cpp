#include "Viewports.h"

Viewports::Viewports()
{
  float xOffset = ofGetWidth() / 3.0f;
  float yOffset = ofGetHeight() / N_CAMERAS;

  viewTop.x = xOffset;
  viewTop.y = 0;
  viewTop.width = xOffset * 2;
  viewTop.height = yOffset;

  viewMain.x = xOffset;
  viewMain.y = yOffset;
  viewMain.width = xOffset * 2;
  viewMain.height = yOffset;
}
