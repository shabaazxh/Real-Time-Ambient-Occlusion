#version 450

layout(binding = 0) uniform SSAOubo {
	vec4 samples[64];
	int sample_amount;
	float radius;

	int hbaoSampleDirection;
	float hbaoSteps;
	int hbaoNumberOfSteps;
	float hbaoAmbientLightLevel;

	int alchemySampleTurns;
	float alchemySigma;
	float alchemyKappa;
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

const vec2 noiseScale = vec2(1920.0/4.0, 1080.0/4.0);

//(ZaOniRinku, 2021) Use depth to obtain normal data
vec3 depthToPositions(vec2 tc)
{
    float depth = texture(depthMap, tc).x;
    vec4 clipSpace = vec4(tc * 2.0 - 1.0, depth, 1.0);
    vec4 viewSpace = inverse(camera.proj) * clipSpace;
    return viewSpace.xyz / viewSpace.w;
}

//(ZaOniRinku, 2021) Use depth to obtain normal data
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

    // Obtain the fragment view space position
    vec3 viewSpacePositions = depthToPositions(uvCoords);
    // Obtain the fragment normal position from view space
    vec4 viewSpaceNormals = depthToNormals(uvCoords);
    // Sample random vector from texture. Apply noise scale to scale across width and height of display
    vec3 randomVec = texture(texNoise, uvCoords * noiseScale).xyz;

    //(Joey De Vries, 2020) Create a TBN matrix to convert the sample from tangent-space to view-space
    vec3 tangent = normalize(randomVec - viewSpaceNormals.xyz * dot(randomVec, viewSpaceNormals.xyz));
    vec3 bitangent = cross(viewSpaceNormals.xyz, tangent);
    mat3 TBN = mat3(tangent, bitangent, viewSpaceNormals.xyz);

    // create the plane to reflect as suggested by (Mittring, 2007)
    vec3 plane = texture(texNoise, uvCoords * noiseScale).xyz - vec3(1.0);

    float occlusion = 0.0;
    for(int i = 0; i < ssao.sample_amount; ++i) {
        
        vec3 samplePos = reflect(ssao.samples[i].xyz, plane); //reflect the sample 
        samplePos = TBN * samplePos; // convert sample to view-space
        samplePos = viewSpacePositions + samplePos * radius; //offset current position with sample pos

        vec4 offset = vec4(samplePos, 1.0);
        offset = camera.proj * offset; //convert .to clip space
        offset.xyz /= offset.w; // perspective divide 
        offset.xy = offset.xy * 0.5 + 0.5; // convert to texture coordinate (0,1)

        float sampleDepth = texture(gPosition, offset.xy).z; // obtain sample pos depth value
        float rangeCheck = (viewSpacePositions.z - sampleDepth) < radius ? 1.0 : 0.0; // range check to ensure within radius
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck; // check if depth > z pos to determine occlusion
    }
    // subtract 1.0 to allow AO to be used with other lighting calculations
    occlusion = 1.0 - (occlusion / ssao.sample_amount);
    outColor = occlusion;
}