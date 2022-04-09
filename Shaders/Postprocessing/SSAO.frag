#version 450


layout(binding = 0) uniform SSAOubo {
    vec4 samples[64];
    int sample_amount;
    float radius;
}ssao;

layout(binding = 4) uniform CameraProjection {
    mat4 model;
    mat4 view;
    mat4 proj;
}camera;

layout(binding = 1) uniform sampler2D texNoise;
layout(binding = 2) uniform sampler2D gPosition;
layout(binding = 3) uniform sampler2D depthMap;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 uvCoords;

layout(location = 0) out float outColor;

float zNear = 0.1;
float zFar = 1000;

int samples = 64;
const vec2 noiseScale = vec2(1920.0/4.0, 1080.0/4.0);

vec3 depthToPositions(vec2 tc)
{
    float depth = texture(depthMap, tc).x;
    vec4 clipSpace = vec4(tc * 2.0 - 1.0, depth, 1.0);
    vec4 viewSpace = inverse(camera.proj) * clipSpace;
    return viewSpace.xyz / viewSpace.w;
}

vec4 depthToNormals(vec2 tc)
{
    float depth = texture(depthMap, tc).x;

    vec4 clipSpace = vec4(tc * 2.0 - 1.0, depth, 1.0);
    vec4 viewSpace = inverse(camera.proj) * clipSpace;
    viewSpace.xyz /= viewSpace.w;

    vec3 pos = viewSpace.xyz;
    vec3 n = normalize(cross(dFdx(pos), dFdy(pos)));
    n *= - 1;

    return vec4(n, 1.0);
}

void main()
{
    float radius = ssao.radius;
    float bias = 0.025;

    vec3 viewSpacePositions = depthToPositions(uvCoords);
    vec4 viewSpaceNormals = depthToNormals(uvCoords);
    vec3 randomVec = texture(texNoise, uvCoords * noiseScale).xyz;

    vec3 tangent = normalize(randomVec - viewSpaceNormals.xyz * dot(randomVec, viewSpaceNormals.xyz));
    vec3 bitangent = cross(viewSpaceNormals.xyz, tangent);
    mat3 TBN = mat3(tangent, bitangent, viewSpaceNormals.xyz);

    vec3 plane = texture(texNoise, uvCoords * noiseScale).xyz - vec3(1.0);

    float occlusion = 0.0;
    for(int i = 0; i < ssao.sample_amount; ++i) {

        vec3 samplePos = reflect(TBN * ssao.samples[i].xyz, plane);
        samplePos = viewSpacePositions + samplePos * radius;

        vec4 offset = vec4(samplePos, 1.0);
        offset = camera.proj * offset;
        offset.xyz /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;
        

        float sampleDepth = texture(gPosition, offset.xy).z;

        float rangeCheck = (viewSpacePositions.z - sampleDepth) < radius ? 1.0 : 0.0;
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / samples);
    outColor = occlusion;
}