#pragma once
#include <ofMain.h>

#include "Grid.h"
#include "IObject.h"
#include "glm/gtx/intersect.hpp"

// Modified version of https://github.com/edap/ofxRaycaster/ by Davide Prati

class Ray
{
public:
  Ray() {};

  Ray(glm::vec3 _origin, glm::vec3 _direction);

  void setup(glm::vec3 _origin, glm::vec3 _direction);

  const glm::vec3 getOrigin();
  void setOrigin(glm::vec3 _origin);
  const glm::vec3 getDirection();
  void setDirection(glm::vec3 _direction);
  void setFromCamera(const glm::vec2& mouse, const ofCamera& camera, const ofRectangle viewport);
  bool intersectsPrimitive(const of3dPrimitive& primitive, float& distance);
  bool intersectsMesh(const ofMesh& mesh, const glm::mat4& transformationMatrix, float& distance, glm::vec3& intNormal);
  glm::vec2 getGridCoordinate(glm::vec2 cursor, ofCamera camera, ofRectangle viewport, Grid grid, int scale);
  int getObjectIndex(glm::vec2 cursor, ofCamera camera, ofRectangle viewport, std::vector<IObject*> objects);
  void drawLaser(glm::vec2 cursor, ofCamera camera, ofRectangle viewport, std::vector<IObject*> objects, Grid grid);

private:
  glm::vec3 origin;
  glm::vec3 direction;
};
