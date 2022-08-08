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

void main() {


    gl_Position = light.LightSpaceMatrix * camera.model * vec4(inPosition, 1.0);

}