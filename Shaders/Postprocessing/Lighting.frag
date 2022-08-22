#version 450

layout(binding = 0) uniform Light {
    vec4 LightPosition;
    vec4 ObjectColor;
    vec4 LightColor;
    vec4 LightViewPosition;
    mat4 LightSpaceMatrix;
    vec4 CameraPosition;
}light;

layout(binding = 4) uniform CameraProjection {
    mat4 model;
    mat4 view;
    mat4 proj;
}camera;

layout(binding = 1) uniform sampler2D gPosition;
layout(binding = 2) uniform sampler2D gNormal;
layout(binding = 3) uniform sampler2D ShadowMapDepth;
layout(binding = 5) uniform sampler2D depthMap;
layout(binding = 6) uniform sampler2D AmbientOcclusion;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 uvCoords;
layout(location = 2) in vec4 FragPosLightSpace;
layout(location = 3) in vec4 FragPos;

layout(location = 0) out vec4 outColor;

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

float Shadow(vec4 fragPosLightSpace)
{
    vec4 positions = vec4(texture(gPosition, uvCoords).xyz, 1.0);
    positions = inverse(camera.view) * positions;

    vec4 fragmentposition = vec4(camera.model * vec4(positions.xyz, 1.0));
    fragPosLightSpace = light.LightSpaceMatrix * vec4(fragmentposition.xyz, 1.0);
    // convert light-space position into the range [-1, 1]
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // convert the coordinates from [-1, 1] to [0,1] in order to correctly sample from depth map
    projCoords.xy = projCoords.xy * 0.5 + 0.5;

    float closestDepth = texture(ShadowMapDepth, projCoords.xy).r;

    float currentDepth = projCoords.z;

    float bias = 0.005;
    float shadow = currentDepth > closestDepth + bias ? 1.0 : 0.0;

    return shadow;
}

// Blinn-phong like lighting model based on (Joey De Vries, 2020) implementation
void main() {

    vec3 FragPos = texture(gPosition, uvCoords).rgb;
    vec3 Normal = texture(gNormal, uvCoords).rgb;
    //float AO = texture(AmbientOcclusion, uvCoords).r;
    float AO = 0.3;
    float ambientValue = (0.3 * 1.0 * AO);
    vec3 ambient = ambientValue * light.LightColor.xyz;

    vec3 lightingDirection = normalize(light.LightPosition.xyz - FragPos.xyz);
    float diffCalc = max(dot(Normal, lightingDirection), 0.0);
    vec3 diffuse = diffCalc * light.LightColor.xyz;

    //specular
    float specularStrength = 0.5; 
    vec4 cameraPosition = vec4(camera.view * light.CameraPosition);   
    vec3 viewDir = normalize(vec3(0,0,0) - FragPos.xyz);
    vec3 reflectDir = reflect(-lightingDirection, Normal.xyz);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light.LightColor.xyz;

    float shadow = Shadow(FragPosLightSpace);

    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * light.ObjectColor.xyz;
    outColor = vec4(result, 1.0);

}