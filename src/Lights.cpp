#include "Lights.h"

Lights::Lights()
{
  light.setDirectional();
  light.setAmbientColor(ofColor(180, 180, 180));
  light.setDiffuseColor(ofColor(128, 128, 128));
  light.setSpecularColor(ofColor(128, 128, 128));
  light.setPosition(0, 0, -100);
  light.enable();
  light.setDirectional();
}
