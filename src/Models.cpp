#include "Models.h"

Models::Models()
{
  Armadillo.loadModel(armadilloPath);
  Armadillo.setScale(0.03, -0.03, 0.03);
  Armadillo.setRotation(0, 180, 0, 1, 0);

  Chest.loadModel(chestPath);
  Chest.setScale(0.02, -0.02, 0.02);
  Chest.setRotation(0, -90, 0, 1, 0);

  Torch.loadModel(torchPath);
  Torch.setScale(0.04, -0.04, 0.04);
}