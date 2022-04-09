#version 450

layout(binding = 0) uniform Light {
    vec4 LightPosition;
    vec4 ObjectColor;
    vec4 LightColor;
    vec4 LightViewPosition;
    mat4 LightSpaceMatrix;
}light;

layout(binding = 1) uniform sampler2D gPosition;
layout(binding = 2) uniform sampler2D gNormal;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 uvCoords;

layout(location = 0) out vec4 outColor;


void main() {

    vec3 FragPos = texture(gPosition, uvCoords).rgb;
    vec3 Normal = texture(gNormal, uvCoords).rgb;

    float ambientValue = 0.1;
    vec3 ambient = ambientValue * light.LightColor.xyz;

    vec3 lightingDirection = normalize(light.LightPosition.xyz - FragPos);
    float diffCalc = max(dot(Normal, lightingDirection), 0.0);
    vec3 diffuse = diffCalc * light.LightColor.xyz;

    vec3 result = (ambient + diffuse) * light.ObjectColor.xyz;

    outColor = vec4(result, 1.0);

}