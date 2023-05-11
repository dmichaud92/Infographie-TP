#include "Chest.h"

void Chest::draw(glm::vec4 pos)
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

std::string Chest::getType()
{
  return "chest";
}