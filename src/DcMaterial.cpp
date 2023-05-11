#include "DcMaterial.h"

DcMaterial::DcMaterial(ofJson json)
{
  setAmbientColor(ofFloatColor(json["ambient"]["r"], json["ambient"]["g"], json["ambient"]["b"], json["ambient"]["a"]));
  setDiffuseColor(ofFloatColor(json["diffuse"]["r"], json["diffuse"]["g"], json["diffuse"]["b"], json["diffuse"]["a"]));
  setEmissiveColor(
    ofFloatColor(json["emissive"]["r"], json["emissive"]["g"], json["emissive"]["b"], json["emissive"]["a"]));
  setSpecularColor(
    ofFloatColor(json["specular"]["r"], json["specular"]["g"], json["specular"]["b"], json["specular"]["a"]));
  setShininess(float(json["shininess"]));
}

DcMaterial::DcMaterial(ofColor color)
{
  ofColor ambient = color;
  ambient.setBrightness(color.getBrightness() - color.getBrightness() / 10);
  setAmbientColor(ambient);

  ofColor diffuse = color;
  diffuse.setBrightness(color.getBrightness() + color.getBrightness() / 20);
  setDiffuseColor(diffuse);

  ofColor specular = color;
  specular.setBrightness(color.getBrightness() + color.getBrightness() / 5);
  setSpecularColor(specular);

  setShininess(100.0f);
}

ofJson DcMaterial::toJson() const
{
  ofJson json;

  ofFloatColor ambient = getAmbientColor();
  ofFloatColor diffuse = getDiffuseColor();
  ofFloatColor emissive = getEmissiveColor();
  ofFloatColor specular = getSpecularColor();

  json["ambient"]["r"] = ambient.r;
  json["ambient"]["g"] = ambient.g;
  json["ambient"]["b"] = ambient.b;
  json["ambient"]["a"] = ambient.a;

  json["diffuse"]["r"] = diffuse.r;
  json["diffuse"]["g"] = diffuse.g;
  json["diffuse"]["b"] = diffuse.b;
  json["diffuse"]["a"] = diffuse.a;

  json["emissive"]["r"] = emissive.r;
  json["emissive"]["g"] = emissive.g;
  json["emissive"]["b"] = emissive.b;
  json["emissive"]["a"] = emissive.a;

  json["specular"]["r"] = specular.r;
  json["specular"]["g"] = specular.g;
  json["specular"]["b"] = specular.b;
  json["specular"]["a"] = specular.a;

  json["shininess"] = getShininess();

  return json;
}