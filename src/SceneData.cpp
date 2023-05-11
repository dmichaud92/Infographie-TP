#pragma once
#include "SceneData.h"

#include <sstream>

#include "Application.h"
void SceneData::save(Grid& grid)
{
  ofJson appData;
  appData["scale"] = scale;
  appData["gridColumns"] = grid.columns;
  appData["gridRows"] = grid.rows;
  appData["gridActive"] = grid.isActive;
  ofSaveJson("Save/appdata.json", appData);

  ofJson json;
  for each (auto& obj in m_sceneGraph)
  {
    json.push_back(obj->toJson());
  }
  ofSaveJson("Save/objects.json", json);
}

void SceneData::load(Grid& grid)
{
  deleteScene(grid);

  ofJson appData = ofLoadJson("Save/appdata.json");
  scale = appData["scale"];
  grid.columns = appData["gridColumns"];
  grid.rows = appData["gridRows"];
  grid.isActive = appData["gridActive"];

  ofJson json = ofLoadJson("Save/objects.json");
  for (ofJson obj : json)
  {
    createObject(obj);
  }
}

std::vector<std::string> SceneData::getSceneGraph() const
{
  std::vector<std::string> list;
  if (m_sceneGraph.size() == 0)
  {
    list.push_back("No Objects");
    return list;
  }
  for each (auto& obj in m_sceneGraph)
  {
    std::string line = obj->toString();
    list.push_back(line);
  }
  return list;
}

void SceneData::selectObjectIndex(int index)
{
  if (m_sceneGraph.size() > index && index >= 0)
    m_selectedObjectIndex = index;
  else
    m_selectedObjectIndex = -1;
}

int SceneData::getSelectedObjectIndex() const
{
  if (m_sceneGraph.size() > m_selectedObjectIndex && m_selectedObjectIndex >= 0)
    return m_selectedObjectIndex;
  else
    return 0;
}

IObject* SceneData::getObject(int index) const
{
  return m_sceneGraph[index];
}

IObject* SceneData::getSelectedObject() const
{
  if (m_sceneGraph.size() > m_selectedObjectIndex && m_selectedObjectIndex >= 0)
    return m_sceneGraph[m_selectedObjectIndex];
  else
    return nullptr;
}

void SceneData::dragObjectIndex(int index)
{
  if (m_sceneGraph.size() > index && index >= 0)
    m_draggedObjectIndex = index;
  else
    m_draggedObjectIndex = -1;
}

IObject* SceneData::getDraggedObject() const
{
  if (m_sceneGraph.size() > m_draggedObjectIndex && m_draggedObjectIndex >= 0)
    return m_sceneGraph[m_draggedObjectIndex];
  else
    return nullptr;
}

void SceneData::createObject(ofJson& json, bool trackHistory)
{
  if (findObject(json) != -1)
    return;
  ObjectTypes type = json["type"];
  switch (type)
  {
    case cube:
      m_sceneGraph.push_back(new Cube(json));
      break;
    case sphere:
      m_sceneGraph.push_back(new Sphere(json));
      break;
    case chest:
      m_sceneGraph.push_back(new Chest(json, m_modelsRef.Chest));
      break;
    case torch:
      m_sceneGraph.push_back(new Torch(json, m_modelsRef.Torch));
      break;
    case armadillo:
      m_sceneGraph.push_back(new Armadillo(json, m_modelsRef.Armadillo));
      break;
    default:
      break;
  }
  if (trackHistory)
    addToHistory(created, json);
}

void SceneData::createObject(IObject* object, bool trackHistory)
{
  if (findObject(object) != -1)
    return;
  m_sceneGraph.push_back(object);
  if (trackHistory)
    addToHistory(created, object->toJson());
}

void SceneData::deleteObject(int index, bool trackHistory)
{
  if (m_sceneGraph.size() > index && index >= 0)
  {
    if (trackHistory)
      addToHistory(deleted, m_sceneGraph[index]->toJson());
    m_sceneGraph.erase(std::next(m_sceneGraph.begin(), index));
  }
}

void SceneData::deleteScene(Grid& grid)
{
  m_sceneGraph.clear();
  m_redoStack.clear();
  m_undoStack.clear();
  m_snapshot = ofJson {};
  m_selectedObjectIndex = -1;
  scale = 30;
  grid = Grid {};
}

void SceneData::addToHistory(Operations operation, ofJson& object)
{
  m_undoStack.push_back(HistoryObject {operation, object});
  m_redoStack.clear();
}

void SceneData::createSnapshot()
{
  const auto& object = getSelectedObject();
  if (object != nullptr)
  {
    m_snapshot = object->toJson();
  }
}

void SceneData::checkForModifications()
{
  const auto& object = getSelectedObject();
  if (object != nullptr)
  {
    ofJson newJson = object->toJson();
    if (newJson == m_snapshot || m_snapshot.is_null())
      return;

    ofJson historyJson;
    historyJson["new"] = newJson;
    historyJson["old"] = m_snapshot;

    addToHistory(modified, historyJson);
    m_snapshot = ofJson {};
  }
}

void SceneData::undo()
{
  if (m_undoStack.size() > 0)
  {
    HistoryObject event = m_undoStack.back();
    m_redoStack.push_back(event);
    if (event.operation == created)
    {
      int index = findObject(event.json);
      if (index >= 0)
        deleteObject(index, false);
    }
    if (event.operation == deleted)
    {
      createObject(event.json, false);
    }
    if (event.operation == modified)
    {
      ofJson oldJson = event.json["old"];
      ofJson newJson = event.json["new"];
      int index = findObject(newJson);
      deleteObject(index, false);
      createObject(oldJson, false);
    }
    m_undoStack.pop_back();
  }
}

void SceneData::redo()
{
  if (m_redoStack.size() > 0)
  {
    HistoryObject event = m_redoStack.back();
    m_undoStack.push_back(event);
    if (event.operation == created)
    {
      createObject(event.json, false);
    }
    if (event.operation == deleted)
    {
      int index = findObject(event.json);
      if (index >= 0)
      {
        deleteObject(index, false);
      }
    }
    if (event.operation == modified)
    {
      ofJson oldJson = event.json["old"];
      ofJson newJson = event.json["new"];
      int index = findObject(oldJson);
      deleteObject(index, false);
      createObject(newJson, false);
    }
    m_redoStack.pop_back();
  }
}

void SceneData::cleanOffGrid(Grid& grid)
{
  std::vector<IObject*> keep;
  for (int i = 0; i < m_sceneGraph.size(); i++)
  {
    glm::vec2 coord = m_sceneGraph[i]->getCoord();
    if (coord.x < grid.columns && coord.y < grid.rows)
    {
      keep.push_back(m_sceneGraph[i]);
    }
  }
  m_sceneGraph = keep;
}

int SceneData::findObject(ofJson& json) const
{
  for (int i = 0; i < m_sceneGraph.size(); i++)
  {
    if (m_sceneGraph[i]->getCoord() == glm::vec2(json["column"], json["row"]))
    {
      return i;
    }
  }
  return -1;
}

int SceneData::findObject(IObject* object) const
{
  for (int i = 0; i < m_sceneGraph.size(); i++)
  {
    if (m_sceneGraph[i]->getCoord() == object->getCoord())
    {
      return i;
    }
  }
  return -1;
}

int SceneData::findObject(glm::vec2& coord) const
{
  for (int i = 0; i < m_sceneGraph.size(); i++)
  {
    if (m_sceneGraph[i]->getCoord() == coord)
    {
      return i;
    }
  }
  return -1;
}

int SceneData::findObject(glm::vec3& intersection) const
{
  // ToDo
  return -1;
}

void SceneData::setTypeObject(int type)
{
  m_objectType = type;
}

int SceneData::getTypeObject()
{
  return m_objectType;
}

std::vector<IObject*> SceneData::getSceneData() const
{
  return m_sceneGraph;
}
