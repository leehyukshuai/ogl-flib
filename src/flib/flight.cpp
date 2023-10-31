#include "flight.hpp"
#include "fscene.hpp"

FLight::FLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, FNode* parent)
    : _ambient(ambient), _diffuse(diffuse), _specular(specular), FNode(parent) {}

FLight::FLight(const FMaterial& material, FNode* parent)
    : _ambient(material._ambient), _diffuse(material._diffuse), _specular(material._specular), FNode(parent) {}

void FLight::render(FScene *scene) {
    scene->program().set(name() + ".ambient", _ambient);
    scene->program().set(name() + ".diffuse", _diffuse);
    scene->program().set(name() + ".specular", _specular);
}

void FLight::setAmbient(const glm::vec3& ambient) {
    _ambient = ambient;
}

void FLight::setDiffuse(const glm::vec3& diffuse) {
    _diffuse = diffuse;
}

void FLight::setSpecular(const glm::vec3& specular) {
    _specular = specular;
}

FDirectLight::FDirectLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction, FNode* parent)
    : FLight(ambient, diffuse, specular, parent), _direction(direction) {}

FDirectLight::FDirectLight(const FMaterial& material, glm::vec3 direction, FNode* parent)
    : FLight(material, parent), _direction(direction) {}

void FDirectLight::render(FScene *scene) {
    scene->program().set(name() + ".direction", glm::normalize((glm::mat3)(scene->trans(this) * _trans) * _direction));
    FLight::render(scene);
}

void FDirectLight::setDirection(const glm::vec3& direction) {
    _direction = direction;
}

FPointLight::FPointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, FNode* parent)
    : FLight(ambient, diffuse, specular, parent), _constant(constant), _linear(linear), _quadratic(quadratic) {}

FPointLight::FPointLight(const FMaterial& material, float constant, float linear, float quadratic, FNode* parent)
    : FLight(material, parent), _constant(constant), _linear(linear), _quadratic(quadratic) {}

void FPointLight::render(FScene *scene) {
    scene->program().set(name() + ".position", glm::vec3((glm::mat4)scene->trans(this) * glm::vec4(position(), 1.0)));
    scene->program().set(name() + ".constant", _constant);
    scene->program().set(name() + ".linear", _linear);
    scene->program().set(name() + ".quadratic", _quadratic);
    FLight::render(scene);
}

void FPointLight::setAttenuation(const glm::vec3& attenuation) {
    _constant = attenuation.x;
    _linear = attenuation.y;
    _quadratic = attenuation.z;
}

FSpotLight::FSpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction, float innerCutoff, float outterCutoff, float constant, float linear, float quadratic, FNode* parent)
    : FPointLight(ambient, diffuse, specular, constant, linear, quadratic, parent), _direction(direction), _innerCutoff(innerCutoff), _outterCutoff(outterCutoff) {}

FSpotLight::FSpotLight(const FMaterial& material, glm::vec3 direction, float innerCutoff, float outterCutoff, float constant, float linear, float quadratic, FNode* parent)
    : FPointLight(material, constant, linear, quadratic, parent), _direction(direction), _innerCutoff(innerCutoff), _outterCutoff(outterCutoff) {}

void FSpotLight::render(FScene *scene) {
    glm::vec3 d = glm::normalize((glm::mat3)(_trans * scene->trans(this)) * _direction);
    scene->program().set(name() + ".direction", glm::normalize((glm::mat3)(_trans * scene->trans(this)) * _direction));
    scene->program().set(name() + ".innerCutoff", glm::cos(glm::radians(_innerCutoff)));
    scene->program().set(name() + ".outterCutoff", glm::cos(glm::radians(_outterCutoff)));
    FPointLight::render(scene);
}

void FSpotLight::setDirection(const glm::vec3& direction) {
    _direction = direction;
}

void FSpotLight::setCutOff(float innerDegrees, float outterDegrees) {
    _innerCutoff = glm::cos(glm::radians(innerDegrees));
    _outterCutoff = glm::cos(glm::radians(outterDegrees));
}

std::string FDirectLight::type() const {
    return "DirectLight";
}

std::string FPointLight::type() const {
    return "PointLight";
}

std::string FSpotLight::type() const {
    return "SpotLight";
}
