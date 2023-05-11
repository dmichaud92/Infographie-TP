#include <ofMain.h>

#include "Application.h"
//========================================================================
int main()
{
  ofGLWindowSettings windowSettings;
  windowSettings.setSize(1920, 1080);
  windowSettings.setGLVersion(3, 3);
  windowSettings.windowMode = OF_WINDOW;
  ofCreateWindow(windowSettings);
  ofRunApp(new Application());
}
