#pragma once
#include <ofMain.h>
#include <ofxAssimpModelLoader.h>

class Models
{
public:
  Models();

  ofxAssimpModelLoader Armadillo;
  ofxAssimpModelLoader Chest;
  ofxAssimpModelLoader Torch;

private:
  std::string armadilloPath = "models/armadillo.obj";
  std::string chestPath = "models/chest.obj";
  std::string torchPath = "models/torch.obj";
};
