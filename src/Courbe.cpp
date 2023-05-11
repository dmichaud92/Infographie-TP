#include "Courbe.h"

void Courbe::addSphere(glm::vec4 pos)
{
  if (nmbrDeSpheres == 0)
  {
    p0 = glm::vec3(pos.x, pos.y, pos.z);
    nmbrDeSpheres += 1;
  }
  else if (nmbrDeSpheres == 1)
  {
    p1 = glm::vec3(pos.x, pos.y, pos.z);
    nmbrDeSpheres += 1;
  }
  else if (nmbrDeSpheres == 2)
  {
    p2 = glm::vec3(pos.x, pos.y, pos.z);
    nmbrDeSpheres += 1;
  }
  else if (nmbrDeSpheres == 3)
  {
    p3 = glm::vec3(pos.x, pos.y, pos.z);
    nmbrDeSpheres += 1;
  }
  else if (nmbrDeSpheres == 4)
  {
    p4 = glm::vec3(pos.x, pos.y, pos.z);
    nmbrDeSpheres += 1;
  }
}

void Courbe::drawCourbe()
{
  if (nmbrDeSpheres == 5)
  {
    ofSetColor(255);
    ofNoFill();
    ofPushMatrix();

    // Dessiner la courbe de Bezier
    ofBeginShape();
    for (float t = 0; t <= 1; t += 0.01)
    {
      glm::vec3 point = pow(1 - t, 4) * p0 + 4 * pow(1 - t, 3) * t * p1 + 6 * pow(1 - t, 2) * pow(t, 2) * p2 +
                        4 * (1 - t) * pow(t, 3) * p3 + pow(t, 4) * p4;
      ofVertex(point);
    }
    ofEndShape();

    ofPopMatrix();
  }
}

void Courbe::reset()
{
  nmbrDeSpheres = 0;
}