#pragma once

#include <ofJson.h>
#include <ofMain.h>
#include <ofxAssimpModelLoader.h>

#include "DcMaterial.h"

enum ObjectTypes
{
  null,
  cube,
  sphere,
  chest,
  torch,
  armadillo,
};

class IObject : public ofNode
{
public:
  ofJson toJson() const;
  std::string toString() const;
  glm::vec2 getCoord() const;
  void setCoord(glm::vec2 position);
  const ofBoxPrimitive& getBoundingBox();
  void drawBoundingBox();
  void calculateBoundingBox();
  vector<ofMesh> getMeshes();
  glm::mat4 getMatrix();

  virtual void lightsOff() { return; };
  virtual void RadiatinglightOn(glm::vec4 pos) { return; };

  DcMaterial* getMaterial() { return &m_material; }

  virtual void draw(glm::vec4 position) = 0;

  virtual std::string getType() = 0;
  ofLight lightObject;

protected:
  IObject(int column, int row, ofxAssimpModelLoader& model);
  IObject(int column, int row);
  IObject(int column, int row, DcMaterial* material);
  IObject(ofJson json, ofxAssimpModelLoader& model);
  IObject(ofJson json);
  ~IObject() {};
  ofBoxPrimitive m_boundingBox;
  of3dPrimitive m_shape;
  glm::mat4 m_matrix;
  DcMaterial m_material;
  int m_row = 0;
  int m_column = 0;
  ofxAssimpModelLoader* m_model;
  ObjectTypes type = null;
  int lightAttenuation = 0;
};
