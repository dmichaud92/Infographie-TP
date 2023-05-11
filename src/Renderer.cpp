#include "Renderer.h"

Renderer::Renderer(Cameras& cameras,
                   Cursor& cursor,
                   Generator& generator,
                   Grid& grid,
                   Lights& lights,
                   Models& models,
                   SceneData& sceneData,
                   UserInterface& userInterface,
                   Viewports& viewports) :
  cameras(cameras),
  cursor(cursor),
  generator(generator),
  grid(grid),
  lights(lights),
  models(models),
  sceneData(sceneData),
  userInterface(userInterface),
  viewports(viewports)
{
}

void Renderer::setup()
{
  shader.setup();
}

void Renderer::update()
{
  shader.update();
}

void Renderer::draw()
{
  ofEnableLighting();
  ofPushMatrix();
  ofPushStyle();
  courbe.reset();
  int n = 0;

  lightsPosition.clear();
  lightsIntensite.clear();
  lightsCouleur.clear();
  for (int i = 0; i < 4; i++)
  {
    lightsPosition.push_back(glm::vec3(0, 0, 0));
    lightsIntensite.push_back(0.0f);
    lightsCouleur.push_back(ofColor(0, 0, 0, 0));
  }

  for each (auto& obj in sceneData.getSceneData())
  {
    obj->RadiatinglightOn(grid.gridToWorld(obj->getCoord(), sceneData.scale));

    if (obj->getType() == "torch" && n < 4)
    {
      VecteurpositionLumiere = glm::vec4(grid.gridToWorld(obj->getCoord(), sceneData.scale));
      couleurLumiere = ofColor(obj->lightObject.getDiffuseColor());

      lightsPosition[n] = (glm::vec3(VecteurpositionLumiere.x, VecteurpositionLumiere.y, VecteurpositionLumiere.z));
      lightsIntensite[n] = (1.0f);
      lightsCouleur[n] = (couleurLumiere);
      n++;
    }

    if (obj->getType() == "sphere")
      courbe.addSphere(grid.gridToWorld(obj->getCoord(), sceneData.scale));
  }

  {
    // main viewport
    cameras.camMain.begin(viewports.viewMain);

    grid.draw(sceneData.scale);
    if (generator.previewEnabled)
      generator.previewMap(grid, sceneData);

    for each (auto& obj in sceneData.getSceneData())
    {
      shader.begin(lightsPosition, lightsIntensite, lightsCouleur);
      obj->draw(grid.gridToWorld(obj->getCoord(), sceneData.scale));
      shader.end();
      if (sceneData.drawBoundingBox)
      {
        obj->drawBoundingBox();
      }
    }

    courbe.drawCourbe();

    if (showLaser)
    {
      Ray Laser;
      Laser.drawLaser(glm::vec2(cursor.x, cursor.y),
                      cameras.camMain,
                      viewports.viewMain,
                      sceneData.getSceneData(),
                      grid);
    }

    cameras.camMain.end();
  }

  {
    // top viewport
    cameras.camTop.begin(viewports.viewTop);
    grid.draw(sceneData.scale);
    if (generator.previewEnabled)
      generator.previewMap(grid, sceneData);

    for each (auto& obj in sceneData.getSceneData())
    {
      shader.begin(lightsPosition, lightsIntensite, lightsCouleur);
      obj->draw(grid.gridToWorld(obj->getCoord(), sceneData.scale));
      shader.end();
      if (sceneData.drawBoundingBox)
      {
        obj->drawBoundingBox();
      }
    }

    courbe.drawCourbe();

    cameras.camTop.end();
  }

  for each (auto& obj in sceneData.getSceneData())
  {
    obj->lightsOff();
  }

  ofPopStyle();
  ofPopMatrix();
  ofDisableLighting();
}