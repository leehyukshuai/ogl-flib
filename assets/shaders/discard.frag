#version 330 core

// position and direction are in WORLD coordinates!
in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

struct Material {
    bool constant;
    vec3 const_color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    bool diffuse_map_exists;
    sampler2D diffuse_map;
    bool specular_map_exists;
    sampler2D specular_map;
    bool emissive_map_exists;
    sampler2D emissive_map;
};

struct DirectLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 direction;
    float innerCutoff;
    float outterCutoff;
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;

uniform vec3 camera_pos;
uniform int directLightNum;
uniform int pointLightNum;
uniform int spotLightNum;
uniform DirectLight[32] directLights;
uniform PointLight[32] pointLights;
uniform SpotLight[32] spotLights;

vec3 calcDirectLight(DirectLight light, vec3 fragPos, vec3 normal, vec2 texCoord) {
    vec3 viewDir = normalize(camera_pos - fragPos);
    vec3 reflectDir = reflect(light.direction, normal);
    float diff = max(dot(-light.direction, normal), 0);
    float spec = material.shininess == 0 ? 1 : pow(max(dot(reflectDir, viewDir), 0), material.shininess);

    vec3 ambient, diffuse, specular;
    if(material.diffuse_map_exists) {
        diffuse = diff * light.diffuse * vec3(texture2D(material.diffuse_map, texCoord));
        ambient = light.ambient * vec3(texture2D(material.diffuse_map, texCoord));
    } else {
        diffuse = diff * light.diffuse * material.diffuse;
        ambient = light.ambient * material.ambient;
    }
    if(material.specular_map_exists) {
        specular = spec * light.specular * vec3(texture2D(material.specular_map, texCoord));
    } else {
        specular = spec * light.specular * material.specular;
    }

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 fragPos, vec3 normal, vec2 texCoord) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 viewDir = normalize(camera_pos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float diff = max(dot(lightDir, normal), 0);
    float spec = material.shininess == 0 ? 1 : pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    float lightDist = distance(light.position, fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * lightDist + light.quadratic * pow(lightDist, 2));

    vec3 ambient, diffuse, specular;
    if(material.diffuse_map_exists) {
        diffuse = attenuation * diff * light.diffuse * vec3(texture2D(material.diffuse_map, texCoord));
        ambient = light.ambient * vec3(texture2D(material.diffuse_map, texCoord));
    } else {
        diffuse = attenuation * diff * light.diffuse * material.diffuse;
        ambient = attenuation * light.ambient * material.ambient;
    }
    if(material.specular_map_exists) {
        specular = attenuation * spec * light.specular * vec3(texture2D(material.specular_map, texCoord));
    } else {
        specular = attenuation * spec * light.specular * material.specular;
    }

    return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 fragPos, vec3 normal, vec2 texCoord) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 viewDir = normalize(camera_pos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float diff = max(dot(lightDir, normal), 0);
    float spec = material.shininess == 0 ? 1 : pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    float lightDist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * lightDist + light.quadratic * pow(lightDist, 2));
    float intensity = clamp((dot(light.direction, -lightDir) - light.outterCutoff) / (light.innerCutoff - light.outterCutoff), 0, 1);

    vec3 ambient, diffuse, specular;
    if(material.diffuse_map_exists) {
        diffuse = attenuation * intensity * diff * light.diffuse * vec3(texture2D(material.diffuse_map, texCoord));
        ambient = light.ambient * vec3(texture2D(material.diffuse_map, texCoord));
    } else {
        diffuse = attenuation * intensity * diff * light.diffuse * material.diffuse;
        ambient = attenuation * light.ambient * material.ambient;
    }
    if(material.specular_map_exists) {
        specular = attenuation * intensity * spec * light.specular * vec3(texture2D(material.specular_map, texCoord));
    } else {
        specular = attenuation * intensity * spec * light.specular * material.specular;
    }

    return ambient + diffuse + specular;
}

void main() {
    if(material.diffuse_map_exists && texture2D(material.diffuse_map, texCoord).a < 0.1) {
        discard;
    }
    if(material.constant) {
        gl_FragColor = vec4(material.const_color, 1.0);
    } else {
        vec3 result = vec3(0, 0, 0);
        for(int i = 0; i < directLightNum; ++i) {
            result += calcDirectLight(directLights[i], fragPos, normal, texCoord);
        }
        for(int i = 0; i < pointLightNum; ++i) {
            result += calcPointLight(pointLights[i], fragPos, normal, texCoord);
        }
        for(int i = 0; i < spotLightNum; ++i) {
            result += calcSpotLight(spotLights[i], fragPos, normal, texCoord);
        }
        if(material.emissive_map_exists) {
            result += vec3(texture2D(material.emissive_map, texCoord));
        }
        if(material.diffuse_map_exists) {
            gl_FragColor = vec4(result, texture2D(material.diffuse_map, texCoord).a);
        } else {
            gl_FragColor = vec4(result, 1.0);
        }
    }
}
