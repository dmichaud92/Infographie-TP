#include "Sphere.h"

void Sphere::draw(glm::vec4 pos)
{
  m_material.begin();
  ofFill();
  m_boundingBox.set(pos.w + 0.1f, pos.w + 0.05f, pos.w + 0.1f);
  m_boundingBox.setPosition(pos.x, pos.y + 0.1f, pos.z);
  m_shape = ofSpherePrimitive(pos.w / 2, 16);
  m_shape.setPosition(pos.x, pos.y + 0.1f, pos.z);
  m_shape.draw();
  ofNoFill();
  m_material.end();
}

void Sphere::RadiatinglightOn(glm::vec4 pos)
{
  int maxCycle = 500;
  int midCycle = 250;

  if (lightAttenuation >= maxCycle)
  {
    lightAttenuation = 0;
  }
  lightObject.setPointLight();
  lightObject.setDiffuseColor(this->getMaterial()->getEmissiveColor());
  lightObject.setPosition(pos.x, pos.y + 15, pos.z);
  if (lightAttenuation <= midCycle)
  {
    lightObject.setAttenuation(1.2, 0.33, -0.01 + 0.005 * lightAttenuation / midCycle);
  }
  else
  {
    lightObject.setAttenuation(1.2, 0.33, -0.01 + 0.005 * (maxCycle - lightAttenuation) / midCycle);
  }
  lightAttenuation += 1;

  lightObject.enable();
}

void Sphere::lightsOff()
{
  lightObject.disable();
}

std::string Sphere::getType()
{
  return "sphere";
}