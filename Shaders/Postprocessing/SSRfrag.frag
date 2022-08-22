#version 450

layout(binding = 4) uniform CameraProjection {
    mat4 model;
    mat4 view;
    mat4 proj;
}camera;

layout(binding = 0) uniform Light {
    vec4 LightPosition;
    vec4 ObjectColor;
    vec4 LightColor;
    vec4 LightViewPosition;
    mat4 LightSpaceMatrix;
    vec4 CameraPosition;
}light;

layout(binding = 1) uniform sampler2D texNoise;
layout(binding = 2) uniform sampler2D gPosition;
layout(binding = 3) uniform sampler2D depthMap;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 uvCoords;

layout(location = 0) out vec4 outColor;

vec4 depthToPosition(vec2 uv) {

    float depth = texture(depthMap, uv).x;
    vec4 clipSpace = vec4(uv * 2.0 - 1.0, depth, 1.0);
    vec4 viewSpace = inverse(camera.proj) * clipSpace;
    viewSpace.xyz /= viewSpace.w;

    return vec4(vec3(viewSpace), 1.0);
}

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

float maxDistance = 8; // maximum length or magnitude of the reflection ray
float resolution = 0.3; // range from 0-1. 0 = no reflections
int steps = 5;
float thickness = 0.5;

void main()
{
    vec2 texSize = textureSize(gPosition, 0).xy;
    vec2 texCoord = gl_FragCoord.xy / texSize;

    vec4 uv = vec4(0.0);

    vec4 positionFrom = texture(gPosition, texCoord);
    vec3 unitPoisitionFrom = normalize(positionFrom.xyz);
    vec4 normal = depthToNormal(texCoord);
    vec3 pivot = normalize(reflect(unitPoisitionFrom, normal.xyz));

    vec4 positionTo = positionFrom;

    vec4 startView = vec4(positionFrom.xyz + (pivot * 0.0), 1.0);
    vec4 endView = vec4(positionFrom.xyz + (pivot * maxDistance), 1.0);

    vec4 startFrag = startView;
    // project to screen-space
    startFrag = camera.proj * startFrag;
    // peform perspectivie divide
    startFrag.xyz /= startFrag.w;
    // convert screen-space XY to UV
    startFrag.xy = startFrag.xy * 0.5 + 0.5;
    // convert UV to fragment/pixel coordinates
    startFrag.xy *= texSize;


    vec4 endFrag = endView;
    // project to screen-space
    endFrag = camera.proj * endFrag;
    // peform perspectivie divide
    endFrag.xyz /= endFrag.w;
    // convert screen-space XY to UV
    endFrag.xy = endFrag.xy * 0.5 + 0.5;
    // convert UV to fragment/pixel coordinates
    endFrag.xy *= texSize;

    vec2 frag = startFrag.xy;
    uv.xy = frag / texSize;

    // calculate difference between x and y coordinates of end and start fragment
    // gives you how many pixels the ray line occupies
    float deltaX = endFrag.x - startFrag.x;
    float deltaY = endFrag.y - startFrag.y;

    float useX = abs(deltaX) >= abs(deltaY) ? 1.0 : 0.0;
    float delta = mix(abs(deltaY), abs(deltaX), useX) * clamp(resolution, 0.0, 1.0);

    vec2 increment = vec2(deltaX, deltaY) / max(delta, 0.001);

    float search0 = 0;
    float search1 = 0;

    int hit0 = 0;
    int hit1 = 0;

    float view_distance = startView.z; //
    float depth = thickness;

    float i = 0;
    for(i = 0; i < int(delta); ++i)
    {
        frag += increment;
        uv.xy = frag / texSize;
        positionTo = texture(gPosition, uv.xy);


        search1 = mix((frag.y - startFrag.y) / deltaY, (frag.x - startFrag.x) / deltaX, useX);
        search1 = clamp(search1, 0.0, 1.0);

        view_distance = (startView.y * endView.y) / mix(endView.y, startView.y, search1);
        depth = view_distance - positionTo.z;

        if(depth > 0 && depth < thickness){
            hit0 = 1;
            break;
        } else {
            search0 = search1;
        }
    }


    search1 = search0 + ((search1 - search0) / 2.0);

    steps *= hit0;

    for(i = 0; i < steps; ++i)
    {
        frag = mix(startFrag.xy, endFrag.xy, search1);
        uv.xy = frag / texSize;
        positionTo = texture(gPosition, uv.xy);

        view_distance = (startView.y * endView.y) / mix(endView.y, startView.y, search1);
        depth = view_distance - positionTo.z;

        if(depth > 0 && depth < thickness){
            hit1 = 1;
            search1 = search0 + ((search1 - search0) / 2);
        } else {
            float temp = search1;
            search1 = search1 + ((search1 - search0) / 2);
            search0 = temp;
        }
    }

    float visibility = hit1 * positionTo.w 
    * (1 - max(dot(-unitPoisitionFrom, pivot), 0))
    * (1 - clamp(depth / thickness, 0, 1)) 
    * (1 - clamp(length(positionTo - positionFrom) / maxDistance, 0, 1))
    * (uv.x < 0 || uv.x > 1 ? 0 : 1)
    * (uv.y < 0 || uv.y > 1 ? 0 : 1);

    visibility = clamp(visibility, 0, 1);
    uv.ba = vec2(visibility);

    outColor = uv;

}