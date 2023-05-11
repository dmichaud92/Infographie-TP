#pragma once
#include <ofMain.h>

#include "IObject.h"

class Chest : public IObject
{
public:
  Chest(int column, int row, ofxAssimpModelLoader& model) : IObject(column, row, model) { type = chest; };
  Chest(ofJson json, ofxAssimpModelLoader& model) : IObject(json, model) { type = chest; };
  std::string getType();

  void draw(glm::vec4 position) override;
};
