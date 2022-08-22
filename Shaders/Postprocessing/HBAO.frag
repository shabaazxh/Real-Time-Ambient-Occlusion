#version 450

/*
Based on jhk2 hbao implementation
(jhk2, 2019) - Changes needed to be made to complete the algorithm such as rotating the direction angle and jittering samples
the shader was re-worked to become compataible with Vulkan. Parameter to control darkness was also introduced
https://github.com/jhk2/glsandbox/blob/master/kgl/samples/ao/hbao.glsl
*/

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

layout(location = 0) out vec4 outColor;

#define PI 3.1415926535897932384626433832795

float RADIUS = ssao.radius;
float NUMBER_OF_SAMPLING_DIRECTIONS = ssao.hbaoSampleDirection;
float STEP = ssao.hbaoSteps; //0.04
float NUMBER_OF_STEPS = ssao.hbaoNumberOfSteps;
float TANGENT_BIAS = 0.3;

float ao = 0.0;
float occlusion = 0.0;

const vec2 noiseScale = vec2(1920.0/4, 1080.0/4);

//(ZaOniRinku, 2021) Use depth to obtain position data
vec4 depthToPosition(vec2 uv) {

    float depth = texture(depthMap, uv).x;
    vec4 clipSpace = vec4(uv * 2.0 - 1.0, depth, 1.0);
    vec4 viewSpace = inverse(camera.proj) * clipSpace;
    viewSpace.xyz /= viewSpace.w;

    return vec4(vec3(viewSpace), 1.0);
}

//(ZaOniRinku, 2021) Use depth to obtain normal data
vec4 depthToNormal(vec2 tc)
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

//Using noise texture to produce a noise jitter value (Kubisch, 2015)
vec4 GetJitter()
{
    return texture(texNoise, (gl_FragCoord.xy / 4));
}

// Slight modified version of (Kubisch, 2015), we use rotation matrix
vec2 RotateDirectionAngle(vec2 direction, vec2 noise)
{
    // contruct a rotation matrix to rotate the direction 
    mat2 rotationMatrix = mat2(vec2(noise.x, -noise.y), vec2(noise.y, noise.x));
    return direction * rotationMatrix;
}

void main()
{

    // position of current fragment
    vec3 pos = vec3(uvCoords, texture(depthMap, uvCoords).r);
    
    vec4 normal = depthToNormal(uvCoords);
    normal.y = -normal.y;

    vec3 NDC_POS = (2.0 * pos) - 1.0; // normalized device coordinates
    vec4 unprojectPosition = inverse(camera.proj) * vec4(NDC_POS, 1.0);
    vec3 viewPosition = unprojectPosition.xyz / unprojectPosition.w;

    // paper suggests to jitter samples by random offset
    vec3 sampleNoise = texture(texNoise, uvCoords * noiseScale).xyz;
    sampleNoise.xy = sampleNoise.xy * 2.0 - vec2(1.0);

    // split disk into uniform directions
    float samplingDiskDirection = 2 * PI / NUMBER_OF_SAMPLING_DIRECTIONS;
    vec4 Rand = GetJitter();

    for(int i = 0; i < NUMBER_OF_SAMPLING_DIRECTIONS; i++) {

        // use i to get a new direction angle by * given direction 
        float samplingDirectionAngle = i * samplingDiskDirection;
        //jitter direction./ Rotate the direction by a random per-pixel angle by passing in the angle 
        //vec2 samplingDirection = RotateDirection(vec2(cos(samplingDirectionAngle), sin(samplingDirectionAngle)), Rand.xy);

        vec2 samplingDirection = RotateDirectionAngle(vec2(cos(samplingDirectionAngle), sin(samplingDirectionAngle)), Rand.xy);

        //tangent angle : inverse cosine. We give it the direction and normal and use acos to get the angle
        float tangentAngle = acos(dot(vec3(samplingDirection, 0.0), normal.xyz)) - (0.5 * PI) + TANGENT_BIAS;
        float horizonAngle = tangentAngle; //set the horizon angle to the tangent angle to begin with

        vec3 LastDifference = vec3(0);

        // for each direction we step in the direction of that sampling direction to sample
        for(int j = 0; j < NUMBER_OF_STEPS; j++){

            // step forward in the sampling direction
            vec2 stepForward = (Rand.z + float(j+1)) * STEP * samplingDirection;
            // use the stepforward position as an offset from the current fragment position in order to move to that location
            vec2 stepPosition = uvCoords + stepForward;
            // sample at the stepped location to get the depth value
            float steppedLocationZ = texture(depthMap, stepPosition.st).x;
            // complete sample position
            vec3 steppedLocationPosition = vec3(stepPosition, steppedLocationZ);
            //convert to NDC
            vec3 steppedPositionNDC = (2.0 * steppedLocationPosition) - 1.0;
            vec4 SteppedPositionUnProj = inverse(camera.proj) * vec4(steppedPositionNDC, 1.0);
            vec3 viewSpaceSteppedPosition = SteppedPositionUnProj.xyz / SteppedPositionUnProj.w;

            // Now that we have the view-space position of the offset sample point
            // We can check the distance from our current fragment to the offset point

            vec3 diff = viewSpaceSteppedPosition.xyz - viewPosition;
            // If the distance is less than the set radius
            if(length(diff) < RADIUS){

                LastDifference = diff;
                float FoundElevationAngle = atan(diff.z / length(diff.xy));
                // update horizon angle if new found elevation angle is larger
                horizonAngle = max(horizonAngle, FoundElevationAngle);
            }
        }

            float norm = length(LastDifference) / RADIUS;
            float attenuation = 1 - (norm * norm);

            occlusion = clamp(attenuation * (sin(horizonAngle) - sin(tangentAngle)), 0.0, 1.0);
            ao += 1.0 - occlusion * ssao.hbaoAmbientLightLevel; //control AO darkness
    }

    ao /= NUMBER_OF_SAMPLING_DIRECTIONS;
    
    outColor = vec4(ao, ao, ao, 1.0);

}

