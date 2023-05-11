#include "Generator.h"

#include "Cube.h"

Generator::Generator()
{
}

void Generator::loadImage(std::string& path, Grid& grid)
{
  m_image.load(path);
  grid.setImage(m_image);
}

void Generator::parseImage(Grid& grid)
{
  if (m_columns != grid.columns || m_rows != grid.rows)
  {
    m_pixels.clear();
    float width = m_image.getWidth();
    float height = m_image.getHeight();
    for (int y = 0; y < grid.rows; y++)
    {
      for (int x = 0; x < grid.columns; x++)
      {
        PixelData pixel;
        float cellWidth = (width / grid.columns);
        float cellHeight = (height / grid.rows);
        float xPos = (x * cellWidth) + (cellWidth / 2.0f);
        float yPos = (y * cellHeight) + (cellHeight / 2.0f);
        pixel.color = m_image.getColor(xPos, yPos);
        pixel.lightness = pixel.color.getLightness();
        pixel.column = x;
        pixel.row = y;
        m_pixels.push_back(pixel);
      }
    }
    m_columns = grid.columns;
    m_rows = grid.rows;
  }
}

void Generator::clear()
{
  m_pixels.clear();
  m_image.clear();
  previewEnabled = false;
  m_columns = 0;
  m_rows = 0;
}

void Generator::previewMap(Grid& grid, SceneData& sceneData, bool addToScene)
{
  parseImage(grid);
  for each (auto& pixel in m_pixels)
  {
    if (pixel.lightness < threshold)
    {
      glm::vec4 pos = grid.gridToWorld(glm::vec2(pixel.column, pixel.row), sceneData.scale);
      ofSetColor(pixel.color);
      ofFill();
      ofDrawBox(pos.x, pos.y, pos.z, pos.w, pos.w, pos.w);
      ofNoFill();
      if (addToScene)
      {
        sceneData.getSceneGraph().clear();
        sceneData.createObject(new Cube(pixel.column, pixel.row, new DcMaterial(pixel.color)));
      }
    }
  }

  if (addToScene)
  {
    grid.removeImage();
    clear();
  }
}