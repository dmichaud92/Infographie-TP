#pragma once

#include <ofMain.h>

#include <iostream>
#include <vector>

#include "Cameras.h"
#include "Cursor.h"
#include "DcMaterial.h"

class Grid
{
public:
  Grid();
  Grid(int rows, int columns, ofFloatColor color);
  Grid(ofJson json);
  ofJson toJson() const;
  void draw(int scale);
  std::string toString() const;
  void screenToCell(Cursor& cursor, Cameras& cameras, int scale, int offsetX, int offsetZ);
  glm::vec2 screenToWorld(Cursor& cursor);
  glm::vec2 getViewportCenter() const;
  glm::vec2 cursorToGrid(glm::vec2 cursor, int scale, int offsetX, int offsetZ) const;
  glm::vec4 gridToWorld(glm::vec2 grid, int scale) const;
  glm::vec2 worldToGrid(glm::vec3 world, int scale) const;
  ofBoxPrimitive getGridBox();
  DcMaterial* getMaterial() { return &m_material; }
  void setImage(ofImage& image);
  void removeImage();

  bool isActive = true;

  int columns = 24;
  int rows = 16;

private:
  ofBoxPrimitive m_gridBox;
  ofFloatColor m_color {0.0f, 0.8f, 0.3f, 1.0f};
  DcMaterial m_material;
  ofTexture m_image;
  bool m_showImage = false;
  void generateTexture(int scale);
  ofTexture m_texture;
  string m_texturelastData = "";
};