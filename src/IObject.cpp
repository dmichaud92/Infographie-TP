#include "IObject.h"

IObject::IObject(int column, int row, ofxAssimpModelLoader& model) : m_column(column), m_row(row), m_model(&model)
{
}

IObject::IObject(int column, int row) : m_column(column), m_row(row), m_model(nullptr)
{
}

IObject::IObject(int column, int row, DcMaterial* material) :
  m_column(column),
  m_row(row),
  m_material(*material),
  m_model(nullptr)
{
}

IObject::IObject(ofJson json, ofxAssimpModelLoader& model)
{
  m_column = json["column"];
  m_row = json["row"];
  m_material = DcMaterial(json["material"]);
  m_model = &model;
}

IObject::IObject(ofJson json)
{
  m_column = json["column"];
  m_row = json["row"];
  m_material = DcMaterial(json["material"]);
  m_model = nullptr;
}

ofJson IObject::toJson() const
{
  ofJson json;

  json["type"] = type;
  json["column"] = m_column;
  json["row"] = m_row;
  json["material"] = m_material.toJson();
  return json;
}

std::string IObject::toString() const
{
  std::stringstream ss;
  switch (type)
  {
    case cube:
      ss << "Cube";
      break;
    case sphere:
      ss << "Sphere";
      break;
    case chest:
      ss << "Chest";
      break;
    case torch:
      ss << "Torch";
      break;
    case armadillo:
      ss << "Armadillo";
      break;
    default:
      break;
  }
  ss << " (" << m_column << "," << m_row << ")";
  return ss.str();
}

glm::vec2 IObject::getCoord() const
{
  return glm::vec2(m_column, m_row);
}

void IObject::setCoord(glm::vec2 position)
{
  m_column = position.x;
  m_row = position.y;
}

void IObject::calculateBoundingBox()
{
  if (m_model == nullptr)
    return;

  glm::vec4 pointMax;
  glm::vec4 pointMin;
  glm::mat4 modelMatrix = ofMatrix4x4::getTransposedOf(m_model->getModelMatrix());
  glm::vec3 min = m_model->getSceneMin();
  glm::vec3 max = m_model->getSceneMax();

  pointMax.x = max.x;
  pointMax.y = max.y;
  pointMax.z = max.z;
  pointMax.w = 1;
  pointMax = pointMax * modelMatrix;

  pointMin.x = min.x;
  pointMin.y = min.y;
  pointMin.z = min.z;
  pointMin.w = 1;
  pointMin = pointMin * modelMatrix;

  float sizeX = pointMax.x - pointMin.x;
  float sizeY = pointMax.y - pointMin.y;
  float sizeZ = pointMax.z - pointMin.z;

  m_boundingBox.set(sizeX, sizeY, sizeZ);
}

vector<ofMesh> IObject::getMeshes()
{
  vector<ofMesh> meshes;
  if (m_model == nullptr)
  {
    ofMesh mesh = m_shape.getMesh();
    meshes.push_back(mesh);
  }
  else
  {
    for (size_t i = 0; i < m_model->getMeshCount(); i++)
    {
      ofMesh mesh = m_model->getMesh(i);
      meshes.push_back(mesh);
    }
  }
  return meshes;
}

glm::mat4 IObject::getMatrix()
{
  if (m_model == nullptr)
  {
    return m_shape.getGlobalTransformMatrix();
  }
  else
  {
    return m_matrix;
  }
}

const ofBoxPrimitive& IObject::getBoundingBox()
{
  calculateBoundingBox();
  return m_boundingBox;
}

void IObject::drawBoundingBox()
{
  ofPushStyle();
  ofSetColor(255);
  getBoundingBox().drawWireframe();
  ofPopStyle();
}

std::string IObject::getType()
{
  return "null";
}
