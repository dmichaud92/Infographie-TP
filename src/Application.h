#pragma once

#include <ofMain.h>

#include "Cameras.h"
#include "Cursor.h"
#include "Generator.h"
#include "Grid.h"
#include "Lights.h"
#include "Models.h"
#include "Ray.h"
#include "Renderer.h"
#include "SceneData.h"
#include "UserInterface.h"
#include "Viewports.h"

class Application : public ofBaseApp
{
public:
  Cameras cameras;
  Cursor cursor;
  Generator generator;
  Grid grid;
  Lights lights;
  Models models;
  SceneData sceneData {models};
  UserInterface userInterface;
  Viewports viewports;
  Renderer renderer {cameras, cursor, generator, grid, lights, models, sceneData, userInterface, viewports};

  bool keyUp = false;
  bool keyDown = false;
  bool keyLeft = false;
  bool keyRight = false;
  bool keyPlus = false;
  bool keyMinus = false;
  bool keyw = false;
  bool keya = false;
  bool keys = false;
  bool keyd = false;
  bool keyW = false;
  bool keyA = false;
  bool keyS = false;
  bool keyD = false;
  bool keyr = false;
  bool keyR = false;
  bool keyCtrl = false;
  bool keySpace = false;
  bool keyShift = false;

  const int topCameraSpeed = 200;
  const int mainCameraSpeed = 200;
  const int zoomSpeed = 15;

  bool hideCursor = false;

  void resetKeys();
  void handleMovement();

  float time_current;
  float time_last;
  float time_elapsed;

  void setup();
  void update();
  void draw();

  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseDragged(int x, int y, int button);

  void mouseLeftPress();
  void mouseLeftRelease();
  void mouseLeftDragged();

  void mouseMiddlePress();
  void mouseMiddleRelease();
  void mouseMiddleDragged();

  void mouseRightPress();
  void mouseRightRelease();
  void mouseRightDragged();

  void keyPressed(int key);
  void keyReleased(int key);

  void mouseMoved(int x, int y);
  void windowResized(int w, int h);
};
