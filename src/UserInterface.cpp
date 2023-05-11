#include "UserInterface.h"

UserInterface::UserInterface()
{
  gui.setup();
}

void UserInterface::draw(SceneData& sceneData, Grid& grid, Viewports& viewports, Cursor& cursor, Generator& generator)
{
  // Remove depth to allow GUI on top at all time.
  ofDisableDepthTest();

  auto selectedObject = sceneData.getSelectedObject();

  gui.begin();
  {

    ImGui::Begin("Material Editor");
    {
      if (selectedObject != nullptr)
      {
        float shininess = selectedObject->getMaterial()->getShininess();
        ImGui::Text("Shininess");
        if (ImGui::SliderFloat(" ", &shininess, 0.0f, 1024.0f))
        {
          selectedObject->getMaterial()->setShininess(shininess);
          sceneData.isModified = true;
        }

        ofFloatColor ambient = selectedObject->getMaterial()->getAmbientColor();
        ImGui::Text("Ambient color");
        if (ImGui::ColorPicker4("  ", (float*)&ambient))
        {
          selectedObject->getMaterial()->setAmbientColor(ambient);
          sceneData.isModified = true;
        }

        ofFloatColor diffuse = selectedObject->getMaterial()->getDiffuseColor();
        ImGui::Text("Diffuse color");
        if (ImGui::ColorPicker4("   ", (float*)&diffuse))
        {
          selectedObject->getMaterial()->setDiffuseColor(diffuse);
          sceneData.isModified = true;
        }

        ofFloatColor specular = selectedObject->getMaterial()->getSpecularColor();
        ImGui::Text("Specular color");
        if (ImGui::ColorPicker4("    ", (float*)&specular))
        {
          selectedObject->getMaterial()->setSpecularColor(specular);
          sceneData.isModified = true;
        }

        ofFloatColor emissive = selectedObject->getMaterial()->getEmissiveColor();
        ImGui::Text("Emissive color");
        if (ImGui::ColorPicker4("     ", (float*)&emissive))
        {
          selectedObject->getMaterial()->setEmissiveColor(emissive);
          sceneData.isModified = true;
        }
      }
    }
    ImGui::End();

    ImGui::Begin("Grid");
    {
      static bool inputs_step = true;

      if (ImGui::Checkbox("Show Grid", &grid.isActive))
      {
      }
      if (ImGui::InputScalar("Grid Columns", ImGuiDataType_U16, &grid.columns, inputs_step ? &u16_one : NULL, NULL))
      {
        sceneData.cleanOffGrid(grid);
      }
      if (ImGui::InputScalar("Grid Rows", ImGuiDataType_U16, &grid.rows, inputs_step ? &u16_one : NULL, NULL))
      {
        sceneData.cleanOffGrid(grid);
      }
      if (ImGui::InputScalar("Scene Scale", ImGuiDataType_U16, &sceneData.scale, inputs_step ? &u16_one : NULL, NULL))
      {
      }
    }
    ImGui::End();

    ImGui::Begin("Save");
    {
      static bool inputs_step = true;

      if (ImGui::Button("Save"))
      {
        sceneData.save(grid);
      }
      if (ImGui::Button("Load"))
      {
        sceneData.load(grid);
      }
      if (ImGui::Button("Undo"))
      {
        sceneData.undo();
      }
      if (ImGui::Button("Redo"))
      {
        sceneData.redo();
      }
      if (ImGui::Button("Clear"))
      {
        sceneData.deleteScene(grid);
        generator.clear();
      }
    }
    ImGui::End();

    ImGui::Begin("Model selector");
    {
      static bool inputs_step = true;
      if (ImGui::Checkbox("Show bounding box", &sceneData.drawBoundingBox))
      {
      }
      if (ImGui::Button("Cube"))
      {
        sceneData.setTypeObject(cube);
      }
      if (ImGui::Button("Sphere"))
      {
        sceneData.setTypeObject(sphere);
      }
      if (ImGui::Button("Chest"))
      {
        sceneData.setTypeObject(chest);
      }
      if (ImGui::Button("Torch"))
      {
        sceneData.setTypeObject(torch);
      }
      if (ImGui::Button("Armadillo"))
      {
        sceneData.setTypeObject(armadillo);
      }
    }
    ImGui::End();

    ImGui::Begin("Dungeon Generator");
    {
      if (ImGui::Button("Import Image"))
      {
        ofFileDialogResult result = ofSystemLoadDialog("Load file");
        if (result.bSuccess)
        {
          string path = result.getPath();
          generator.loadImage(path, grid);
          generator.previewEnabled = true;
        }
      }
      if (ImGui::Checkbox("Preview Map", &generator.previewEnabled))
      {
      }
      if (ImGui::SliderFloat("Threshold", &generator.threshold, 0.0f, 255.0f))
      {
      }
      if (ImGui::Button("Generate Map"))
      {
        generator.previewMap(grid, sceneData, true);
      };
    }
    ImGui::End();

    ImGui::Begin("Stats");
    {
      // Save FrameTime into queue
      const double frameTime = ofGetLastFrameTime();
      frames.push_back(frameTime);
      // Calculate Average FrameTime
      double sum = 0;
      for (auto it = frames.begin(); it != frames.end(); ++it)
      {
        sum += *it;
      }
      double avgFrameTime = sum / frames.size();

      // Clear old FrameTimes
      while (frames.size() > (int)ofGetFrameRate())
      {
        frames.pop_front();
      }

      // Output String
      std::stringstream ss;
      ss.precision(4);

      // Output FrameTime
      ss << "Frame Time: " << std::fixed << frameTime << "ms";
      ImGui::Text(ss.str().c_str());
      ss.str("");

      // Output Average FrameTime
      ss << "Average Frame Time: " << std::fixed << avgFrameTime << "ms";
      ImGui::Text(ss.str().c_str());
      ss.str("");

      ss.precision(2);

      // Output FPS
      ss << "FPS: " << std::fixed << 1 / frameTime;
      ImGui::Text(ss.str().c_str());
      ss.str("");

      // Output Average FPS
      ss << "Average FPS: " << std::fixed << 1 / avgFrameTime;
      ImGui::Text(ss.str().c_str());
      ss.str("");

      // ss << "Frames" << std::fixed << ofGetFrameRate();
      // ImGui::Text(ss.str().c_str());
      // ss.str("");
      //
      // ss << "Queue" << std::fixed << frames.size();
      // ImGui::Text(ss.str().c_str());
      // ss.str("");
    }
    ImGui::End();
    ImGui::Begin("Controls");
    {
      ImGui::Text("Controls");
      ImGui::Text(" Left click to add object. ");
      ImGui::Text(" Right click to remove object. ");
      ImGui::Text(" Middle click to move the light source. ");
      ImGui::Text(" ");
      ImGui::Text("Top View Controls");
      ImGui::Text(" ^ v < > ");
      ImGui::Text(" ");
      ImGui::Text("3D View Controls");
      ImGui::Text(" W A S D CTRL SPACE + - ");
      ImGui::Text(" Hold SHIFT for alternate movement. ");
      ImGui::Text(" Hold Right Click to Rotate. ");
      ImGui::Text(" Middle Click for Laser. ");
    }
    ImGui::End();
  }
  gui.end();

  // draw some labels
  ofSetColor(255, 255, 255);
  ofDrawBitmapString("3D View", viewports.viewMain.x + 20, viewports.viewMain.y + 30);
  ofDrawBitmapString("Top View", viewports.viewTop.x + 20, viewports.viewTop.y + 30);

  // draw outlines on views
  ofSetLineWidth(5);
  ofNoFill();
  ofSetColor(255, 255, 255);
  ofDrawRectangle(viewports.viewTop);
  ofDrawRectangle(viewports.viewMain);
  // Restore State
  ofEnableDepthTest();
}
