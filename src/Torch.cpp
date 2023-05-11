#include "Torch.h"

void Torch::draw(glm::vec4 pos)
{
  m_material.begin();
  ofFill();
  m_boundingBox.setPosition(pos.x, m_boundingBox.getHeight() / 2, pos.z);
  m_model->setPosition(pos.x, pos.y - 15, pos.z);
  m_model->draw(OF_MESH_FILL);
  m_matrix = m_model->getModelMatrix();
  ofNoFill();
  m_material.end();
}

void Torch::RadiatinglightOn(glm::vec4 pos)
{
  int red = 255 + ofRandom(-70, 0);
  int green = 110 * red / 255;

  ofColor fireColor(red, green, 0);

  lightObject.setPointLight();
  lightObject.setDiffuseColor(fireColor);
  lightObject.setPosition(pos.x, pos.y + 15, pos.z);
  lightObject.setAttenuation(1.2, 0.001, 0.0001);
  lightObject.enable();
}

void Torch::lightsOff()
{
  lightObject.disable();
}

std::string Torch::getType()
{
  return "torch";
}