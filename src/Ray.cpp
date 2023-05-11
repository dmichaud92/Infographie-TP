#include "Ray.h"

Ray::Ray(glm::vec3 _origin, glm::vec3 _direction)
{
  origin = _origin;
  direction = glm::normalize(_direction);
}

void Ray::setup(glm::vec3 _origin, glm::vec3 _direction)
{
  origin = _origin;
  direction = glm::normalize(_direction);
}

/// \brief it returns the origin of the ray,
const glm::vec3 Ray::getOrigin()
{
  return origin;
}

void Ray::setOrigin(glm::vec3 _origin)
{
  origin = _origin;
}

const glm::vec3 Ray::getDirection()
{
  return direction;
}

void Ray::setDirection(glm::vec3 _direction)
{
  direction = _direction;
}

void Ray::setFromCamera(const glm::vec2& mouse, const ofCamera& camera, const ofRectangle viewport)
{
  glm::vec3 screenMouse(mouse.x, mouse.y, 0);
  auto worldMouse = camera.screenToWorld(screenMouse, viewport);
  auto worldMouseEnd = camera.screenToWorld(glm::vec3(screenMouse.x, screenMouse.y, 1.0f), viewport);

  auto worldMouseDirection = worldMouseEnd - worldMouse;

  setup(camera.getGlobalPosition(), glm::normalize(worldMouseDirection));
}

bool Ray::intersectsPrimitive(const of3dPrimitive& primitive, float& distance)
{
  // at the beginning, no intersection is found and the distance to the closest surface
  // is set to an the max of a float value;
  bool found = false;
  float distanceToTheClosestSurface = numeric_limits<float>::max();
  glm::vec2 baricentricCoordinates;

  for (const ofMeshFace& face : primitive.getMesh().getUniqueFaces())
  {
    bool intersection =
      glm::intersectRayTriangle(origin,
                                direction,
                                glm::vec3(primitive.getGlobalTransformMatrix() * glm::vec4(face.getVertex(0), 1.f)),
                                glm::vec3(primitive.getGlobalTransformMatrix() * glm::vec4(face.getVertex(1), 1.f)),
                                glm::vec3(primitive.getGlobalTransformMatrix() * glm::vec4(face.getVertex(2), 1.f)),
                                glm::vec2(),
                                distance);

    // when an intersection is found, it updates the distanceToTheClosestSurface value
    // this value is used to order the new intersections, if a new intersection with a smaller baricenter.z
    // value is found, this one will become the new intersection
    if (intersection)
    {
      if (distance < distanceToTheClosestSurface)
      {
        found = true;
        distanceToTheClosestSurface = distance;
      }
    }
  }

  distance = distanceToTheClosestSurface;
  return found;
}

bool Ray::intersectsMesh(const ofMesh& mesh,
                         const glm::mat4& transformationMatrix,
                         float& distance,
                         glm::vec3& intNormal)
{
  // Guards. intersectsMesh only works with indexed geometries of
  // traingles
  if (mesh.getMode() != OF_PRIMITIVE_TRIANGLES)
  {
    ofLog() << "intersectsMesh works only with OF_PRIMITIVE_TRIANGLES";
    return false;
  }

  if (mesh.getNumIndices() < 3)
  {
    ofLog() << "mesh intersection works only with indexed geometries";
    return false;
  }

  if (mesh.getNumIndices() % 3 != 0)
  {
    ofLog() << "the total number of the indices is not a multiple of 3";
    return false;
  }

  // at the beginning, no intersection is found and the distance
  // to the closest surface is set to an high value;
  bool found = false;
  float distanceToTheClosestSurface = numeric_limits<float>::max();
  glm::vec3 midpoint;
  for (unsigned int i = 0; i < mesh.getNumIndices(); i += 3)
  {
    glm::vec3 v1 = transformationMatrix * glm::vec4(mesh.getVertex(mesh.getIndex(i)), 1.0f);
    glm::vec3 v2 = transformationMatrix * glm::vec4(mesh.getVertex(mesh.getIndex(i + 1)), 1.0f);
    glm::vec3 v3 = transformationMatrix * glm::vec4(mesh.getVertex(mesh.getIndex(i + 2)), 1.0f);

    bool intersection = glm::intersectRayTriangle(origin, direction, v1, v2, v3, glm::vec2(), distance);

    if (intersection)
    {
      if (distance < distanceToTheClosestSurface)
      {
        found = true;
        distanceToTheClosestSurface = distance;

        glm::vec3 e1 = v1 - v2;
        glm::vec3 e2 = v3 - v2;
        glm::vec4 no = glm::vec4(glm::cross(e1, e2), 1.0f);

        midpoint = (v1 + v2 + v3) / 3;

        intNormal = glm::vec3(transformationMatrix * no);
      }
    }
  }
  // ofDrawLine(midpoint, midpoint + intNormal * 0.5);
  distance = distanceToTheClosestSurface;
  return found;
}

glm::vec2 Ray::getGridCoordinate(glm::vec2 cursor, ofCamera camera, ofRectangle viewport, Grid grid, int scale)
{
  setFromCamera(glm::vec2(cursor.x, cursor.y), camera, viewport);
  float distance;
  float distanceToClosestIntersection = numeric_limits<float>::max();
  bool intersects = intersectsPrimitive(grid.getGridBox(), distance);
  if (intersects && (distance < distanceToClosestIntersection))
  {
    distanceToClosestIntersection = distance;
  }
  if (intersects)
  {
    auto intersection = getOrigin() + getDirection() * distance;
    return grid.worldToGrid(intersection, scale);
  }
  return glm::vec2(-1, -1);
}

int Ray::getObjectIndex(glm::vec2 cursor, ofCamera camera, ofRectangle viewport, std::vector<IObject*> objects)
{
  setFromCamera(glm::vec2(cursor.x, cursor.y), camera, viewport);
  float distance;
  float distanceToClosestIntersection = numeric_limits<float>::max();
  int foundIndex = -1;
  for (auto i = 0; i < objects.size(); i++)
  {
    auto& object = objects.at(i);
    bool intersects = intersectsPrimitive(object->getBoundingBox(), distance);
    if (intersects && (distance < distanceToClosestIntersection))
    {
      distanceToClosestIntersection = distance;
      foundIndex = i;
    }
  }
  return foundIndex;
}

void Ray::drawLaser(glm::vec2 cursor, ofCamera camera, ofRectangle viewport, std::vector<IObject*> objects, Grid grid)
{
  setFromCamera(glm::vec2(cursor.x, cursor.y), camera, viewport);

  float distance = numeric_limits<float>::max();
  float distanceToClosestIntersection = numeric_limits<float>::max();

  bool found = false;
  glm::vec3 normal;

  // Filter the object that the laser won't it the boundingbox.
  vector<IObject*> objectInLine;
  float boundingBoxDistance;
  for (auto i = 0; i < objects.size(); i++)
  {
    auto& object = objects.at(i);
    bool intersects = intersectsPrimitive(object->getBoundingBox(), boundingBoxDistance);
    if (intersects)
    {
      objectInLine.push_back(object);
    }
  }

  if (objectInLine.size() > 0)
  {
    // test against all objects in line.
    for (auto i = 0; i < objectInLine.size(); i++)
    {
      auto& object = objectInLine.at(i);
      vector<ofMesh> meshes = object->getMeshes();
      glm::mat4 matrix = object->getMatrix();
      for (auto i = 0; i < meshes.size(); i++)
      {
        auto& mesh = meshes.at(i);
        glm::vec3 intNormal;
        bool intersects = intersectsMesh(mesh, matrix, distance, intNormal);

        if (intersects && (distance < distanceToClosestIntersection))
        {
          distanceToClosestIntersection = distance;
          normal = intNormal;
          found = true;
        }
      }
    }
  }

  // Test against the plane.
  if (!found)
  {
    bool intersects = intersectsPrimitive(grid.getGridBox(), distance);
    if (intersects && (distance < distanceToClosestIntersection))
    {
      distanceToClosestIntersection = distance;
      found = true;
    }
  }

  // Draw laser if found
  if (found)
  {
    auto intersection = origin + direction * distanceToClosestIntersection;
    ofPushMatrix();
    ofPushStyle();

    ofSetColor(255, 0, 0, 255);
    ofFill();

    glm::vec3 laserOrigin(origin.x, origin.y - 1, origin.z);

    ofDrawLine(laserOrigin, intersection);
    ofDrawSphere(intersection, 1);
    // auto reflLight = glm::reflect(direction, normal);
    // ofSetColor(255, 0, 0, 128);
    // ofDrawLine(intersection, intersection + 100 * reflLight);

    ofPopStyle();
    ofPopMatrix();
  }
}
