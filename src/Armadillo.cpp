#include "Armadillo.h"

void Armadillo::draw(glm::vec4 pos)
{
  m_material.begin();
  ofFill();
  m_boundingBox.setPosition(pos.x, m_boundingBox.getHeight() / 2, pos.z);
  m_model->setPosition(pos.x, pos.y - 15, pos.z);
  m_model->draw(OF_MESH_FILL);
  m_matrix = m_model->getModelMatrix();
  m_material.end();
}

std::string Armadillo::getType()
{
  return "armadillo";
}

void Armadillo::RadiatinglightOn(glm::vec4 pos)
{
  lightObject.setSpotlight();
  lightObject.setDiffuseColor(15);
  lightObject.setPosition(pos.x, pos.y - 5, pos.z);
  lightObject.setOrientation(ofQuaternion(0, 1, 0, 0));
  lightObject.setSpotlightCutOff(25);
  lightObject.setAttenuation(1.0, 0.001, 0.001);
  lightObject.enable();

  lightAttenuation += 1;
}

void Armadillo::lightsOff()
{
  lightObject.disable();
}