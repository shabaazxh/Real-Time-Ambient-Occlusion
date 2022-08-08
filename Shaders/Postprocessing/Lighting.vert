#version 450

layout(binding = 0) uniform Light {
    vec4 LightPosition;
    vec4 ObjectColor;
    vec4 LightColor;
    vec4 LightViewPosition;
    mat4 LightSpaceMatrix;
}light;

layout(binding = 4) uniform CameraProjection {
    mat4 model;
    mat4 view;
    mat4 proj;
}camera;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 texCoords;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 uvCoords;
layout(location = 2) out vec4 FragPosLightSpace;
layout(location = 3) out vec4 FragPos;

void main() {
    
    fragColor = inColor;
    uvCoords = texCoords;
    FragPos = vec4(camera.model * vec4(inPosition, 1.0));
    FragPosLightSpace = light.LightSpaceMatrix * vec4(FragPos.xyz, 1.0);
    gl_Position = vec4(inPosition, 1.0);

}