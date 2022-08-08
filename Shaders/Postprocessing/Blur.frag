#version 450


layout(binding = 0) uniform sampler2D ssao;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 uvCoords;


layout(location = 0) out float outColor;

// Based on (Joey De Vries, 2020) blur implementation.
void main()
{
    vec2 texelSize = 1.0 / vec2(textureSize(ssao, 0));
    float result = 0.0;
    for(int x = -2; x < 2; ++x)
    {
        for(int y = -2; y < 2; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize; //multiply by a single texelsize to convert to 0,1
            result += texture(ssao, uvCoords + offset).r;
        }
    }

    outColor = result / (4.0 * 4.0);
}

