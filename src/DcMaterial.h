#pragma once

#include <ofMain.h>

class DcMaterial : public ofMaterial
{
public:
  DcMaterial() : ofMaterial() {};
  DcMaterial(ofJson json);
  DcMaterial(ofColor color);
  ofJson toJson() const;
};
