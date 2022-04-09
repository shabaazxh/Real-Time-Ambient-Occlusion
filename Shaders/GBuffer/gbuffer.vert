#version 450

layout(binding = 0) uniform CameraProjection {
    mat4 model;
    mat4 view;
    mat4 proj;
}camera;


layout(location = 0) in vec3 inPosition;
layout(location = 3) in vec3 aNormal;

layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec3 Normal;


void main() {
    vec4 viewPos = camera.view * camera.model * vec4(inPosition, 1.0);
    FragPos = viewPos.xyz;

    mat3 normalMatrix = transpose(inverse(mat3(camera.view * camera.model)));
    Normal = normalMatrix * aNormal;

    gl_Position = camera.proj * viewPos;
}