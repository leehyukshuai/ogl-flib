#pragma once
#include "fheaders.hpp"
#include "fprogram.hpp"
#include "fmesh.hpp"
#include "fnode.hpp"

class FLight: public FNode {
protected:
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;
    FLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, FNode* parent = nullptr);
    FLight(const FMaterial& material, FNode* parent = nullptr);

public:
    void render(FScene *scene) override;
    void setAmbient(const glm::vec3& ambient);
    void setDiffuse(const glm::vec3& diffuse);
    void setSpecular(const glm::vec3& specular);

    virtual std::string type()const = 0;
};

class FDirectLight: public FLight {
protected:
    glm::vec3 _direction;

public:
    FDirectLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction, FNode* parent = nullptr);
    FDirectLight(const FMaterial& material, glm::vec3 direction, FNode* parent = nullptr);
    void render(FScene *scene) override;
    void setDirection(const glm::vec3& direction);

    std::string type()const override;
};

class FPointLight: public FLight {
protected:
    float _constant;
    float _linear;
    float _quadratic;

public:
    FPointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant = 1.0f, float linear = 0.07f, float quadratic = 0.017f, FNode* parent = nullptr);
    FPointLight(const FMaterial& material, float constant = 1.0f, float linear = 0.07f, float quadratic = 0.017f, FNode* parent = nullptr);
    void render(FScene *scene) override;
    void setAttenuation(const glm::vec3& attenuation);

    std::string type()const override;
};

class FSpotLight: public FPointLight {
protected:
    glm::vec3 _direction;
    float _innerCutoff;
    float _outterCutoff;

public:
    FSpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction = {}, float innerCutoff = 15.0f, float outterCutoff = 25.0f, float constant = 1.0f, float linear = 0.07f, float quadratic = 0.017f, FNode* parent = nullptr);
    FSpotLight(const FMaterial& material, glm::vec3 direction = {}, float innerCutoff = 15.0f, float outterCutoff = 25.0f, float constant = 1.0f, float linear = 0.07f, float quadratic = 0.017f, FNode* parent = nullptr);
    void render(FScene *scene) override;
    void setDirection(const glm::vec3& direction);
    void setCutOff(float innerDegrees, float outterDegrees);

    std::string type()const override;
};
