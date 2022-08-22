#version 450


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 texCoords;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 uvCoords;

void main() {
    fragColor = inColor;
    uvCoords = texCoords;
    gl_Position = vec4(inPosition, 1.0);

}