#pragma once

#include <ofxImGui.h>

#include <iostream>

#include "Cursor.h"
#include "Generator.h"
#include "SceneData.h"
#include "Viewports.h"

class UserInterface
{
public:
  UserInterface();
  void draw(SceneData& sceneData, Grid& grid, Viewports& viewport, Cursor& cursor, Generator& generator);
  const ImU16 u16_one = 1;
  ofxImGui::Gui gui;

private:
  std::deque<double> frames {};
};
