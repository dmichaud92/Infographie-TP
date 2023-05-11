#pragma once

#include <ofMain.h>

#include <array>
#include <iostream>

#include "IObject.h"

class Courbe
{
public:
  void addSphere(glm::vec4 pos);
  void drawCourbe();
  void reset();

private:
  int m_row1 = 0;
  int m_column1 = 0;
  int m_row2 = 0;
  int m_column2 = 0;
  int m_row3 = 0;
  int m_column3 = 0;
  int m_row4 = 0;
  int m_column4 = 0;
  int m_row5 = 0;
  int m_column5 = 0;
  ofFloatColor m_color = (1.0, 1.0, 1.0, 1.0);
  glm::vec3 p0;
  glm::vec3 p1;
  glm::vec3 p2;
  glm::vec3 p3;
  glm::vec3 p4;
  int nmbrDeSpheres = 0;
};
