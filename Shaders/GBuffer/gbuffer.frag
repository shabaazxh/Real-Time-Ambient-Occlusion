
#version 450


layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec3 Normal;


layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;

void main() {
    gPosition = FragPos;
    gNormal = normalize(Normal);
}