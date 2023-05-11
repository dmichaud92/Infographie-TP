#pragma once
#include <ofMain.h>

#include "IObject.h"

class Torch : public IObject
{
public:
  Torch(int column, int row, ofxAssimpModelLoader& model) : IObject(column, row, model) { type = torch; };
  Torch(ofJson json, ofxAssimpModelLoader& model) : IObject(json, model) { type = torch; };
  std::string getType();

  void draw(glm::vec4 position) override;

  void RadiatinglightOn(glm::vec4 pos) override;
  void lightsOff() override;
};
