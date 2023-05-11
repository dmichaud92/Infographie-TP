#include "Cube.h"

void Cube::draw(glm::vec4 pos)
{
  ofFill();
  m_boundingBox.set(pos.w + 0.1f, pos.w + 0.05f, pos.w + 0.1f);
  m_boundingBox.setPosition(pos.x, pos.y + 0.1f, pos.z);
  m_shape = ofBoxPrimitive(pos.w, pos.w, pos.w);
  m_shape.setPosition(pos.x, pos.y, pos.z);
  m_shape.draw();
  ofNoFill();
}

std::string Cube::getType()
{
  return "cube";
}