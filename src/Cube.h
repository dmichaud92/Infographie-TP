#pragma once
#include <ofMain.h>

#include "IObject.h"

class Cube : public IObject
{

public:
  Cube(int column, int row) : IObject(column, row) { type = cube; };
  Cube(int column, int row, DcMaterial* material) : IObject(column, row, material) { type = cube; };
  Cube(ofJson json) : IObject(json) { type = cube; };
  std::string getType();

  void draw(glm::vec4 position) override;
};
