#include "Grid.h"

Grid::Grid()
{
}

Grid::Grid(int rows, int columns, ofFloatColor color) : rows(rows), columns(columns), m_color(color)
{
}

Grid::Grid(ofJson json)
{
  rows = json["rows"];
  columns = json["columns"];
}

ofJson Grid::toJson() const
{
  ofJson json;

  json["rows"] = rows;
  json["columns"] = columns;

  return json;
}

void Grid::draw(int scale)
{
  float width = (columns * scale);
  float height = (rows * scale);

  float xOffset = width / 2.0f;
  float zOffset = height / 2.0f;

  ofVec3f topLeft(-xOffset, 0.0f, -zOffset);
  ofVec3f topRight(xOffset, 0.0f, -zOffset);
  ofVec3f bottomRight(xOffset, 0.0f, zOffset);
  ofVec3f bottomLeft(-xOffset, 0.0f, zOffset);

  m_gridBox.set(width, 1.0f, height);
  m_gridBox.setPosition(0, -0.51f, 0);

  ofSetColor(0);
  if (isActive)
  {
    for (int i = 0; i <= rows; i++)
    {
      ofVec3f start = {-xOffset, 0, i * scale - zOffset};
      ofVec3f end = {xOffset, 0, i * scale - zOffset};
      ofDrawLine(start, end);
    }

    for (int i = 0; i <= columns; i++)
    {
      ofVec3f start = {i * scale - xOffset, 0, -zOffset};
      ofVec3f end = {i * scale - xOffset, 0, +zOffset};
      ofDrawLine(start, end);
    }
  }

  if (m_showImage)
  {
    m_image.bind();
  }
  else
  {
    if (m_texturelastData != to_string(scale) + to_string(columns) + to_string(rows))
    {
      generateTexture(scale);
      m_texturelastData = to_string(scale) + to_string(columns) + to_string(rows);
    }
    m_texture.bind();
  }

  m_material.begin();
  m_gridBox.draw();
  m_material.end();
  m_image.unbind();
  m_texture.unbind();
}

std::string Grid::toString() const
{
  std::stringstream ss;
  ss << "Grid (" << rows << "," << columns << ")";
  return ss.str();
}

void Grid::screenToCell(Cursor& cursor, Cameras& cameras, int scale, int offsetX, int offsetZ)
{
  glm::vec2 grid = cursorToGrid(glm::vec2(cursor.x, cursor.y), scale, offsetX, offsetZ);
  int cellX = grid.x > 0 && grid.x < columns ? grid.x : -1;
  int cellY = grid.y > 0 && grid.y < rows ? grid.y : -1;

  cursor.x2d = cellX != -1 && cellY != -1 ? cellX : -1;
  cursor.y2d = cellX != -1 && cellY != -1 ? cellY : -1;
}

glm::vec2 Grid::screenToWorld(Cursor& cursor)
{
  auto viewportCenter = getViewportCenter();
  float X = (cursor.x - viewportCenter.x);
  float Y = (cursor.y - viewportCenter.y);
  return glm::vec2(X, Y);
}

glm::vec2 Grid::getViewportCenter() const
{
  float viewportCenterX = (ofGetWidth() / 3.0f) * 2.0f;
  float viewportCenterZ = ofGetHeight() / 4.0f;

  return glm::vec2(viewportCenterX, viewportCenterZ);
}

glm::vec2 Grid::cursorToGrid(glm::vec2 cursor, int scale, int offsetX, int offsetZ) const
{
  auto viewportCenter = getViewportCenter();
  float gridX = (((cursor.x - viewportCenter.x) + offsetX) / scale) + (columns / 2.0f);
  float gridY = (((cursor.y - viewportCenter.y) + offsetZ) / scale) + (rows / 2.0f);
  return glm::vec2(gridX, gridY);
}

glm::vec4 Grid::gridToWorld(glm::vec2 grid, int scale) const
{
  float xOffset = (columns * scale) / 2.0f - (scale / 2.0f);
  float zOffset = (rows * scale) / 2.0f - (scale / 2.0f);
  float worldx = (grid.x * scale - xOffset);
  float worldz = (grid.y * scale - zOffset);
  return glm::vec4(worldx, scale / 2.0f, worldz, scale);
}

glm::vec2 Grid::worldToGrid(glm::vec3 world, int scale) const
{
  float xOffset = (columns * scale) / 2.0f;
  float zOffset = (rows * scale) / 2.0f;
  float gridX = floor((world.x + xOffset) / scale);
  float gridY = floor((world.z + zOffset) / scale);
  return glm::vec2(gridX, gridY);
}

ofBoxPrimitive Grid::getGridBox()
{
  return m_gridBox;
}

void Grid::removeImage()
{
  m_showImage = false;
  m_image = ofTexture {};
}

void Grid::setImage(ofImage& image)
{
  m_image = image.getTexture();
  m_showImage = true;
}

void Grid::generateTexture(int scale)
{
  int textureWidth = scale * columns;
  int textureHeight = scale * rows;
  m_texture.allocate(textureWidth, textureHeight, GL_RGB);

  float* pixels = new float[textureWidth * textureHeight * 3];

  for (int y = 0; y < textureHeight; y++)
  {
    for (int x = 0; x < textureWidth; x++)
    {
      float red = ofRandom(0.3, 0.4);
      float green = ofRandom(0.2, 0.3);
      float blue = ofRandom(0.1, 0.3);

      int index = (y * textureWidth + x) * 3;

      pixels[index] = red;
      pixels[index + 1] = green;
      pixels[index + 2] = blue;
    }
  }

  m_texture.loadData(pixels, textureWidth, textureHeight, GL_RGB);

  delete[] pixels;
}