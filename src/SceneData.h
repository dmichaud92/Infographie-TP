#pragma once
#include <ofMain.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "Armadillo.h"
#include "Chest.h"
#include "Cube.h"
#include "Grid.h"
#include "IObject.h"
#include "Models.h"
#include "Sphere.h"
#include "Torch.h"

enum Operations
{
  created,
  deleted,
  modified
};

struct HistoryObject
{
  Operations operation;
  ofJson json;
};

class SceneData
{
public:
  SceneData(Models& models) : m_modelsRef(models) {};
  void save(Grid& grid);
  void load(Grid& grid);
  void undo();
  void redo();
  void createObject(ofJson& json, bool trackHistory = true);
  void createObject(IObject* object, bool trackHistory = true);
  void deleteObject(int index, bool trackHistory = true);
  void deleteScene(Grid& grid);
  void cleanOffGrid(Grid& grid);
  int findObject(ofJson& json) const;
  int findObject(IObject* object) const;
  int findObject(glm::vec2& coord) const;
  int findObject(glm::vec3& intersection) const;
  void addToHistory(Operations operation, ofJson& object);
  void createSnapshot();
  void checkForModifications();
  std::vector<std::string> getSceneGraph() const;
  void selectObjectIndex(int index);
  int getSelectedObjectIndex() const;
  IObject* getObject(int index) const;
  IObject* getSelectedObject() const;
  void dragObjectIndex(int index);
  IObject* getDraggedObject() const;
  void setTypeObject(int type);
  int getTypeObject();
  std::vector<IObject*> getSceneData() const;

  int scale = 30;
  int offsetX = 0;
  int offsetZ = 0;
  bool isModified = false;

  bool drawBoundingBox = false;

private:
  Models& m_modelsRef;
  int m_selectedObjectIndex = -1;
  int m_draggedObjectIndex = -1;
  std::vector<IObject*> m_sceneGraph;
  std::vector<HistoryObject> m_undoStack;
  std::vector<HistoryObject> m_redoStack;
  ofJson m_snapshot;
  int m_objectType = null;
};
