#include "Application.h"

void Application::setup()
{
  ofSetWindowTitle("Dungeon Creator");
  // ofSetLogLevel(OF_LOG_VERBOSE);

  ofSetFrameRate(144);
  ofSetVerticalSync(true);

  ofSetBackgroundColor(0);
  ofHideCursor();
  ofSetSphereResolution(16);

  ofDisableArbTex();
  ofEnableSmoothing();
  ofEnableAlphaBlending();
  ofEnableDepthTest();
  ofEnableLighting();

  lights.light.setPosition(0.0f, 200.0f, 0.0f);

  renderer.setup();
}

void Application::update()
{
  handleMovement();
  renderer.update();
}

void Application::draw()
{
  renderer.draw();
  userInterface.draw(sceneData, grid, viewports, cursor, generator);

  if (!hideCursor)
    cursor.draw();
}

// EVENTS

void Application::mousePressed(int x, int y, int button)
{
  cursor.x = x;
  cursor.y = y;

  switch (button)
  {
    case 0:
      mouseLeftPress();
      break;
    case 1:
      mouseMiddlePress();
      break;
    case 2:
      mouseRightPress();
      break;
    default:
      break;
  }
}

void Application::mouseReleased(int x, int y, int button)
{
  cursor.x = x;
  cursor.y = y;

  switch (button)
  {
    case 0:
      mouseLeftRelease();
      break;
    case 1:
      mouseMiddleRelease();
      break;
    case 2:
      mouseRightRelease();
      break;
    default:
      break;
  }
}

void Application::mouseDragged(int x, int y, int button)
{
  cursor.x = x;
  cursor.y = y;
  grid.screenToCell(cursor, cameras, sceneData.scale, sceneData.offsetX, sceneData.offsetZ);

  switch (button)
  {
    case 0:
      mouseLeftDragged();
      break;
    case 1:
      mouseMiddleDragged();
      break;
    case 2:
      mouseRightDragged();
      break;
    default:
      break;
  }
}

void Application::mouseLeftPress()
{
  cursor.state = cursor.touch;

  glm::vec2 gridPos {-1, -1};
  if (viewports.viewMain.inside(cursor.getRectangle()))
  {
    Ray objectRay;
    glm::vec2 coordinate {-1, -1};
    int objectIndex = objectRay.getObjectIndex(glm::vec2(cursor.x, cursor.y),
                                               cameras.camMain,
                                               viewports.viewMain,
                                               sceneData.getSceneData());

    if (objectIndex > -1)
    {
      sceneData.selectObjectIndex(objectIndex);
      sceneData.dragObjectIndex(objectIndex);
    }
    else
    {
      Ray gridRay;
      coordinate = gridRay.getGridCoordinate(glm::vec2(cursor.x, cursor.y),
                                             cameras.camMain,
                                             viewports.viewMain,
                                             grid,
                                             sceneData.scale);
    }

    if (coordinate.x >= 0 && coordinate.y >= 0)
      gridPos = coordinate;
  }

  if (viewports.viewTop.inside(cursor.getRectangle()))
  {
    if (cursor.x2d >= 0 && cursor.y2d >= 0)
    {
      gridPos.x = cursor.x2d;
      gridPos.y = cursor.y2d;
    }
  }

  if (gridPos.x >= 0 && gridPos.y >= 0)
  {
    int objectType = sceneData.getTypeObject();
    switch (objectType)
    {
      case null:
        break;
      case cube:
        sceneData.createObject(new Cube(gridPos.x, gridPos.y));
        break;
      case sphere:
        sceneData.createObject(new Sphere(gridPos.x, gridPos.y));
        break;
      case chest:
        sceneData.createObject(new Chest(gridPos.x, gridPos.y, models.Chest));
        break;
      case torch:
        sceneData.createObject(new Torch(gridPos.x, gridPos.y, models.Torch));
        break;
      case armadillo:
        sceneData.createObject(new Armadillo(gridPos.x, gridPos.y, models.Armadillo));
        break;
      default:
        break;
    }
    int index = sceneData.findObject(glm::vec2(gridPos.x, gridPos.y));
    sceneData.selectObjectIndex(index);
    sceneData.dragObjectIndex(index);
  }
  sceneData.createSnapshot();
}

void Application::mouseLeftDragged()
{
  cursor.state = cursor.drag;
  if (viewports.viewMain.inside(cursor.getRectangle()))
  {
    Ray ray;
    glm::vec2 coordinate =
      ray.getGridCoordinate(glm::vec2(cursor.x, cursor.y), cameras.camMain, viewports.viewMain, grid, sceneData.scale);

    if (coordinate.x >= 0 && coordinate.y >= 0)
    {
      auto draggedObject = sceneData.getDraggedObject();
      if (draggedObject != nullptr)
        draggedObject->setCoord(coordinate);
    }
  }
  if (viewports.viewTop.inside(cursor.getRectangle()))
  {
    if (cursor.x2d >= 0 && cursor.y2d >= 0)
    {
      auto draggedObject = sceneData.getDraggedObject();
      if (draggedObject != nullptr)
        draggedObject->setCoord(glm::vec2(cursor.x2d, cursor.y2d));
    }
  }
}

void Application::mouseLeftRelease()
{
  cursor.state = cursor.normal;

  sceneData.dragObjectIndex(-1);

  if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) || sceneData.isModified)
  {
    sceneData.checkForModifications();
    sceneData.isModified = false;
  }
}

void Application::mouseMiddlePress()
{
  cursor.state = cursor.bucket;
  if (viewports.viewTop.inside(cursor.getRectangle()))
  {
    glm::vec2 lightPosition = grid.screenToWorld(cursor);
    lights.light.setPosition(lightPosition.x, 200.0f, lightPosition.y);
  }
  if (viewports.viewMain.inside(cursor.getRectangle()))
  {
    renderer.showLaser = true;
    hideCursor = true;
  }
}

void Application::mouseMiddleDragged()
{
  cursor.state = cursor.bucket;
  if (viewports.viewTop.inside(cursor.getRectangle()))
  {
    glm::vec2 lightPosition = grid.screenToWorld(cursor);
    lights.light.setPosition(lightPosition.x, 200.0f, lightPosition.y);
  }
}

void Application::mouseMiddleRelease()
{
  cursor.state = cursor.normal;
  renderer.showLaser = false;
  hideCursor = false;
}

void Application::mouseRightPress()
{
  cursor.state = cursor.touch;
  if (viewports.viewMain.inside(cursor.getRectangle()))
  {
    hideCursor = true;
    Ray objectRay;
    int objectIndex = objectRay.getObjectIndex(glm::vec2(cursor.x, cursor.y),
                                               cameras.camMain,
                                               viewports.viewMain,
                                               sceneData.getSceneData());

    if (objectIndex > -1)
    {
      sceneData.deleteObject(objectIndex);
    }
  }
  if (viewports.viewTop.inside(cursor.getRectangle()))
  {
    if (cursor.x2d >= 0 && cursor.y2d >= 0)
    {
      int index = sceneData.findObject(glm::vec2(cursor.x2d, cursor.y2d));
      sceneData.deleteObject(index);
    }
  }
}

void Application::mouseRightDragged()
{
  cursor.state = cursor.drag;
  if (viewports.viewMain.inside(cursor.getRectangle()))
  {

    // Camera movement
    ofPushMatrix();

    float mouseSensitivity = 1.5f;
    float mouseMovementX = ((ofGetMouseX() - ofGetPreviousMouseX()) / viewports.viewMain.width) * mouseSensitivity;
    float mouseMovementY = ((ofGetMouseY() - ofGetPreviousMouseY()) / viewports.viewMain.height) * mouseSensitivity;

    float camPitch = cameras.camMain.getPitchRad();
    float camHeading = cameras.camMain.getHeadingRad();
    float camRoll = cameras.camMain.getRollRad();

    double newCamPitch = camPitch - mouseMovementY;
    double newCamHeading;

    // flip direction at -90 and 90
    if (camRoll >= -(PI / 2) && camRoll <= PI / 2)
    {
      newCamHeading = camHeading - mouseMovementX;
    }
    else
    {
      newCamHeading = camHeading + mouseMovementX;
    }

    glm::highp_vec3 directionVector(newCamPitch, newCamHeading, camRoll);
    glm::quat cameraDirection(directionVector);

    cameras.camMain.setOrientation(cameraDirection);

    ofPopMatrix();
  }
}

void Application::mouseRightRelease()
{
  cursor.state = cursor.normal;
  hideCursor = false;
}

void Application::mouseMoved(int x, int y)
{
  cursor.x = x;
  cursor.y = y;

  grid.screenToCell(cursor, cameras, sceneData.scale, sceneData.offsetX, sceneData.offsetZ);
}

void Application::keyPressed(int key)
{
  // cout << key << endl;
  switch (key)
  {
    case OF_KEY_UP:
      keyUp = true;
      break;
    case OF_KEY_DOWN:
      keyDown = true;
      break;
    case OF_KEY_LEFT:
      keyLeft = true;
      break;
    case OF_KEY_RIGHT:
      keyRight = true;
      break;
    case 43:
      keyPlus = true;
      break;
    case 45:
      keyMinus = true;
      break;
    case 119:
      keyw = true;
      break;
    case 97:
      keya = true;
      break;
    case 115:
      keys = true;
      break;
    case 100:
      keyd = true;
      break;
    case 87:
      keyW = true;
      break;
    case 65:
      keyA = true;
      break;
    case 83:
      keyS = true;
      break;
    case 68:
      keyD = true;
      break;
    case 114:
      keyr = true;
      break;
    case 82:
      keyR = true;
      break;
    case OF_KEY_LEFT_CONTROL:
      keyCtrl = true;
      break;
    case 32:
      keySpace = true;
      break;
    case OF_KEY_LEFT_SHIFT:
      keyShift = true;
      break;
    default:
      break;
  }
}

void Application::keyReleased(int key)
{
  switch (key)
  {
    case OF_KEY_UP:
      keyUp = false;
      break;
    case OF_KEY_DOWN:
      keyDown = false;
      break;
    case OF_KEY_LEFT:
      keyLeft = false;
      break;
    case OF_KEY_RIGHT:
      keyRight = false;
      break;
    case 43:
      keyPlus = false;
      break;
    case 45:
      keyMinus = false;
      break;
    case 119:
      keyw = false;
      break;
    case 97:
      keya = false;
      break;
    case 115:
      keys = false;
      break;
    case 100:
      keyd = false;
      break;
    case 87:
      keyW = false;
      break;
    case 65:
      keyA = false;
      break;
    case 83:
      keyS = false;
      break;
    case 68:
      keyD = false;
      break;
    case 114:
      keyr = false;
      break;
    case 82:
      keyR = false;
      break;
    case OF_KEY_LEFT_CONTROL:
      keyCtrl = false;
      break;
    case 32:
      keySpace = false;
      break;
    case OF_KEY_LEFT_SHIFT:
      keyShift = false;
      break;
    default:
      break;
  }
}

void Application::resetKeys()
{
  keyUp = false;
  keyDown = false;
  keyLeft = false;
  keyRight = false;
  keyPlus = false;
  keyMinus = false;
  keyw = false;
  keya = false;
  keys = false;
  keyd = false;
  keyW = false;
  keyA = false;
  keyS = false;
  keyD = false;
  keyr = false;
  keyR = false;
  keyCtrl = false;
  keySpace = false;
  keyShift = false;
}

void Application::handleMovement()
{
  // Time interpolation
  time_current = ofGetElapsedTimef();
  time_elapsed = time_current - time_last;
  time_last = time_current;

  // Top View Camera Movement.
  glm::vec3 topPosition = cameras.camTop.getGlobalPosition();

  if (keyR || keyr)
  {
    resetKeys();
    cameras.resetMainCamera();
  }

  if (keyUp)
  {
    cameras.camTop.setGlobalPosition(topPosition.x, topPosition.y, topPosition.z - topCameraSpeed * time_elapsed);
  }

  if (keyDown)
  {
    cameras.camTop.setGlobalPosition(topPosition.x, topPosition.y, topPosition.z + topCameraSpeed * time_elapsed);
  }

  if (keyRight)
  {
    cameras.camTop.setGlobalPosition(topPosition.x + topCameraSpeed * time_elapsed, topPosition.y, topPosition.z);
  }

  if (keyLeft)
  {
    cameras.camTop.setGlobalPosition(topPosition.x - topCameraSpeed * time_elapsed, topPosition.y, topPosition.z);
  }

  // update grid offset
  sceneData.offsetX = topPosition.x;
  sceneData.offsetZ = topPosition.z;

  // 3D view Camera Movement.
  glm::vec3 mainPosition = cameras.camMain.getGlobalPosition();

  if (keyPlus)
  {
    cameras.camMain.setFov(cameras.camMain.getFov() - zoomSpeed * time_elapsed);
  }

  if (keyMinus)
  {
    cameras.camMain.setFov(cameras.camMain.getFov() + zoomSpeed * time_elapsed);
  }

  if (keyW)
  {
    cameras.camMain.setGlobalPosition(mainPosition.x, mainPosition.y, mainPosition.z - mainCameraSpeed * time_elapsed);
  }
  if (keyw)
  {
    cameras.camMain.move(cameras.camMain.getLookAtDir() * mainCameraSpeed * time_elapsed);
  }

  if (keyS)
  {
    cameras.camMain.setGlobalPosition(mainPosition.x, mainPosition.y, mainPosition.z + mainCameraSpeed * time_elapsed);
  }
  if (keys)
  {
    cameras.camMain.move(cameras.camMain.getLookAtDir() * -mainCameraSpeed * time_elapsed);
  }

  if (keyD)
  {
    cameras.camMain.setGlobalPosition(mainPosition.x + mainCameraSpeed * time_elapsed, mainPosition.y, mainPosition.z);
  }
  if (keyd)
  {
    cameras.camMain.move(cameras.camMain.getSideDir() * mainCameraSpeed * time_elapsed);
  }

  if (keyA)
  {
    cameras.camMain.setGlobalPosition(mainPosition.x - mainCameraSpeed * time_elapsed, mainPosition.y, mainPosition.z);
  }
  if (keya)
  {
    cameras.camMain.move(cameras.camMain.getSideDir() * -mainCameraSpeed * time_elapsed);
  }

  if (keyShift && keySpace)
  {
    cameras.camMain.setGlobalPosition(mainPosition.x, mainPosition.y + mainCameraSpeed * time_elapsed, mainPosition.z);
  }
  if (!keyShift && keySpace)
  {
    cameras.camMain.move(cameras.camMain.getUpDir() * mainCameraSpeed * time_elapsed);
  }

  if (keyShift && keyCtrl)
  {
    cameras.camMain.setGlobalPosition(mainPosition.x, mainPosition.y - mainCameraSpeed * time_elapsed, mainPosition.z);
  }
  if (!keyShift && keyCtrl)
  {
    cameras.camMain.move(cameras.camMain.getUpDir() * -mainCameraSpeed * time_elapsed);
  }
}

void Application::windowResized(int w, int h)
{
  viewports = Viewports();
}