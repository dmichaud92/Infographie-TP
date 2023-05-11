#pragma once
#include <ofMain.h>

#include "IObject.h"

class Armadillo : public IObject
{
public:
  Armadillo(int column, int row, ofxAssimpModelLoader& model) : IObject(column, row, model) { type = armadillo; };
  Armadillo(ofJson json, ofxAssimpModelLoader& model) : IObject(json, model) { type = armadillo; };
  std::string getType();

  void draw(glm::vec4 position) override;
  void RadiatinglightOn(glm::vec4 pos) override;
  void lightsOff() override;
};