
#version 450


layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec3 Normal;


layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;

// G-buffer understanding and output to MRT was built from (Joey De Vries, 2020) implementation of G-buffer output.
void main() {
    gPosition = FragPos;
    gNormal = normalize(Normal);
}