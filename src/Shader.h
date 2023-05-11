#pragma once
#include <ofMain.h>

class Shader : public ofShader
{
public:
  // Shader() : ofShader() {};
  void setup();
  void update();
  // void reset();
  void begin(std::vector<glm::vec3>, std::vector<float>, std::vector<ofColor>);
  void end();

  // should be private maybe
  ofColor material_color_ambient;
  ofColor material_color_diffuse;
  ofColor material_color_specular;

  float material_metallic = 0.0f;
  float material_roughness = 0.0f;
  float material_occlusion = 0.0f;
  float material_brightness = 0.0f;

  glm::vec3 material_fresnel_ior;

  ofImage texture_diffuse;
  ofImage texture_metallic;
  ofImage texture_roughness;
  ofImage texture_occlusion;
  ofImage texture_normalMap;

  // pour test

  ofShader shader;
  float delta_x;
  float delta_y;
  float delta_z;

  int offsetX = 0;
  int offsetZ = 0;

  float rotation_y;

  float initial_x;
  float initial_z;

  float center_x;
  float center_y;

  ofLight light;
  ofLight light2;
  ofLight light3;
  ofLight light4;

  glm::vec3 light_position;
  glm::vec3 light_position2;
  glm::vec3 light_position3;
  glm::vec3 light_position4;

  ofColor light_color;
  ofColor light_color2;
  ofColor light_color3;
  ofColor light_color4;

  float light_intensity = 0.0f;
  float light_intensity2 = 0.0f;
  float light_intensity3 = 0.0f;
  float light_intensity4 = 0.0f;

  bool light_motion = false;

  float tone_mapping_exposure = 0.0f;
  float tone_mapping_gamma = 0.0f;
  bool tone_mapping_toggle = false;
};