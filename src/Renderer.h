#pragma once
#include <ofMain.h>

#include "Cameras.h"
#include "Courbe.h"
#include "Cursor.h"
#include "Generator.h"
#include "Grid.h"
#include "Lights.h"
#include "Models.h"
#include "Ray.h"
#include "Renderer.h"
#include "SceneData.h"
#include "Shader.h"
#include "UserInterface.h"
#include "Viewports.h"

class Renderer
{
public:
  Renderer(Cameras& cameras,
           Cursor& cursor,
           Generator& generator,
           Grid& grid,
           Lights& lights,
           Models& models,
           SceneData& sceneData,
           UserInterface& userInterface,
           Viewports& viewports);

  void setup();
  void update();
  void draw();
  glm::vec4 VecteurpositionLumiere;
  ofColor couleurLumiere;
  ofColor couleurNull = (0, 0, 0, 0);

  std::vector<glm::vec3> lightsPosition;
  std::vector<float> lightsIntensite;
  std::vector<ofColor> lightsCouleur;

  Shader shader;
  bool showLaser = false;

private:
  UserInterface& userInterface;
  Cameras& cameras;
  Viewports& viewports;
  Cursor& cursor;
  Lights& lights;
  Models& models;
  SceneData& sceneData;
  Grid& grid;
  Generator& generator;
  Courbe courbe;
};
