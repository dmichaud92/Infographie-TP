#pragma once

#include <ofMain.h>

#include "Grid.h"
#include "SceneData.h"

struct PixelData
{
  int column;
  int row;
  ofColor color;
  float lightness;
};

class Generator
{
public:
  Generator();
  void loadImage(std::string& path, Grid& grid);
  void previewMap(Grid& grid, SceneData& sceneData, bool addToScene = false);
  void parseImage(Grid& grid);
  void clear();
  float threshold = 50.0f;
  bool previewEnabled = false;

private:
  ofImage m_image;
  std::vector<PixelData> m_pixels;
  int m_columns = 0;
  int m_rows = 0;
};