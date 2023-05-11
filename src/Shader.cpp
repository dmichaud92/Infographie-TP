#include "Shader.h"

void Shader::setup()
{
  // Update those dynamically based on the material
  shader.load("shaders/pbr_relief_vs.glsl", "shaders/pbr_relief_fs.glsl");
  center_x = ofGetWidth() / 2.0f;
  center_y = ofGetHeight() / 2.0f;

  texture_diffuse.load("models/textures/rustic_stone_wall_diff_1k.jpg");
  texture_metallic.load("models/textures/rustic_stone_wall_arm_1k.jpg");
  texture_roughness.load("models/textures/rustic_stone_wall_rough_1k.jpg");
  texture_occlusion.load("models/textures/rustic_stone_wall_ao_1k.jpg");
  texture_normalMap.load("models/textures/rustic_stone_wall_nor_gl_1k.jpg");

  texture_diffuse.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_metallic.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_roughness.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_occlusion.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_normalMap.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);

  // reset();

  // parametres du materiau
  material_color_ambient = ofColor(45, 45, 45);
  material_color_diffuse = ofColor(255, 255, 255);
  material_color_specular = ofColor(255, 255, 255);

  material_metallic = 0.3f;
  material_roughness = 0.9f;
  material_occlusion = 0.5f;
  material_brightness = 1.2f;

  material_fresnel_ior = glm::vec3(0.04f, 0.04f, 0.04f);

  light_motion = true;

  // parametres de mappage tonal
  tone_mapping_exposure = 1.0f;
  tone_mapping_toggle = true;
  tone_mapping_gamma = 1.5f;
}

void Shader::update()
{
  center_x = ofGetWidth() / 2.0f;
  center_y = ofGetHeight() / 2.0f;

  light.setGlobalPosition(light_position);

  light2.setGlobalPosition(light_position2);

  light3.setGlobalPosition(light_position3);

  light4.setGlobalPosition(light_position4);

  shader.begin();
  shader.setUniform3f("material_color_ambient",
                      material_color_ambient.r / 255.0f,
                      material_color_ambient.g / 255.0f,
                      material_color_ambient.b / 255.0f);
  shader.setUniform3f("material_color_diffuse",
                      material_color_diffuse.r / 255.0f,
                      material_color_diffuse.g / 255.0f,
                      material_color_diffuse.b / 255.0f);
  shader.setUniform3f("material_color_specular",
                      material_color_specular.r / 255.0f,
                      material_color_specular.g / 255.0f,
                      material_color_specular.b / 255.0f);

  shader.setUniform1f("material_brightness", material_brightness);
  shader.setUniform1f("material_metallic", material_metallic);
  shader.setUniform1f("material_roughness", material_roughness);
  shader.setUniform1f("material_occlusion", material_occlusion);

  shader.setUniform3f("material_fresnel_ior", material_fresnel_ior);

  shader.setUniformTexture("texture_diffuse", texture_diffuse.getTexture(), 1);
  shader.setUniformTexture("texture_metallic", texture_metallic.getTexture(), 2);
  shader.setUniformTexture("texture_roughness", texture_roughness.getTexture(), 3);
  shader.setUniformTexture("texture_occlusion", texture_occlusion.getTexture(), 4);
  shader.setUniformTexture("texture_normalMap", texture_normalMap.getTexture(), 5);

  // modifier les parametres pour prendre des vecteurs avec les infos des lumieres

  shader.setUniform1f("light_attenuation_factor_constant", 0.02);
  shader.setUniform1f("light_attenuation_factor_linear", 0.00003);
  shader.setUniform1f("light_attenuation_factor_quadratic", 0.000003);

  shader.setUniform1f("light[0].intensity", light_intensity);
  shader.setUniform3f("light[0].color", light_color.r / 255.0f, light_color.g / 255.0f, light_color.b / 255.0f);
  shader.setUniform3f("light[0].position", light.getGlobalPosition());

  shader.setUniform1f("light[1].intensity", light_intensity2);
  shader.setUniform3f("light[1].color", light_color2.r / 255.0f, light_color2.g / 255.0f, light_color2.b / 255.0f);
  shader.setUniform3f("light[1].position", light2.getGlobalPosition());

  shader.setUniform1f("light[2].intensity", light_intensity3);
  shader.setUniform3f("light[2].color", light_color3.r / 255.0f, light_color3.g / 255.0f, light_color3.b / 255.0f);
  shader.setUniform3f("light[2].position", light3.getGlobalPosition());

  shader.setUniform1f("light[3].intensity", light_intensity4);
  shader.setUniform3f("light[3].color", light_color4.r / 255.0f, light_color4.g / 255.0f, light_color4.b / 255.0f);
  shader.setUniform3f("light[3].position", light4.getGlobalPosition());

  shader.setUniform1f("tone_mapping_exposure", tone_mapping_exposure);
  shader.setUniform1f("tone_mapping_gamma", tone_mapping_gamma);
  shader.setUniform1i("tone_mapping_toggle", tone_mapping_toggle);
  shader.end();
}

void Shader::begin(std::vector<glm::vec3> position, std::vector<float> intensite, std::vector<ofColor> couleur)
{
  light_color = couleur[0];
  light_color2 = couleur[1];
  light_color3 = couleur[2];
  light_color4 = couleur[3];

  light_intensity = intensite[0];
  light_intensity2 = intensite[1];
  light_intensity3 = intensite[2];
  light_intensity4 = intensite[3];

  light_position = position[0];
  light_position2 = position[1];
  light_position3 = position[2];
  light_position4 = position[3];

  shader.begin();
}

void Shader::end()
{
  shader.end();
}
