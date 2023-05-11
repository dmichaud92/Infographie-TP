#pragma once
#include <ofMain.h>

#include "IObject.h"

class Sphere : public IObject
{
public:
  Sphere(int column, int row) : IObject(column, row) { type = sphere; };
  Sphere(ofJson json) : IObject(json) { type = sphere; };
  void draw(glm::vec4 position) override;
  std::string getType();

  void RadiatinglightOn(glm::vec4 pos) override;
  void lightsOff() override;
};
