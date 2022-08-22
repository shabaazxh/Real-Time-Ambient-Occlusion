
#version 450

layout(binding = 0) uniform sampler2D SSAOBlurRender;
layout(binding = 1) uniform sampler2D SceneRender;
layout(binding = 2) uniform sampler2D SSAO;
layout(binding = 4) uniform sampler2D Lightingpass;

layout(binding = 3) uniform RenderPresentSettings {
    bool enableBlur;
    bool enableLight;
}presentSettings;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 uvCoords;

layout(location = 0) out vec4 outColor;


void main() {

    vec3 SceneImage = texture(SceneRender, uvCoords).rgb;
    vec3 SceneLighting = texture(Lightingpass, uvCoords).rgb;
    
    // if(presentSettings.enableBlur) {
    //     float SSAOImage = texture(SSAOBlurRender, uvCoords).r;
    //     SceneImage *= SSAOImage;
    // } else {
    //     float SSAOImage = texture(SSAO, uvCoords).r;
    //     SceneImage *= SSAOImage;
    // }
    vec3 result= SceneImage *= SceneLighting;
    outColor = vec4(SceneLighting, 1.0);

}