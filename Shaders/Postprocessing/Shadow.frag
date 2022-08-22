#version 450


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 uvCoords;


layout(location = 0) out vec4 FragColor;

void main()
{   
    gl_FragDepth = gl_FragCoord.z;
}
