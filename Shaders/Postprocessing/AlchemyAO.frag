#version 450

const float PI = 3.14159265359;

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

float sampleRadius = ssao.radius;
int samples = ssao.alchemySampleTurns; 
int turns = 16;
float depthThreshold = 0.000;
float shadowScalar = ssao.alchemySigma; 
float shadowContrast = ssao.alchemyKappa;
const float epsilon = 0.0001;

vec3 depthToPosition(vec2 tc)
{
	float depth = texture(depthMap, tc).x;
	vec4 clipSpace = vec4(tc * 2.0 - 1.0, depth, 1.0);
	vec4 viewSpace = inverse(camera.proj) * clipSpace;
	return viewSpace.xyz / viewSpace.w;
}

vec4 depthToNormal(vec2 tc)
{
	float depth = texture(depthMap, tc).x;
	vec4 clipSpace = vec4(tc * 2.0 - 1.0, depth, 1.0);
	vec4 viewSpace = inverse(camera.proj) * clipSpace;
	viewSpace.xyz / viewSpace.w;
	
	vec3 pos = viewSpace.xyz;
	vec3 n = normalize(cross(dFdx(pos), dFdy(pos)));
	n *= -1;

	return vec4(n, 1.0);
}

void main()
{
	// Normals and positions in view-space
	vec3 Position = depthToPosition(uvCoords);
	vec3 Normal = depthToNormal(uvCoords).xyz;

	float ambientValue = 0.0;
	float screen_radius = (sampleRadius * 100.0 / Position.z);
	
	int max_mip = textureQueryLevels(gPosition) - 1;
	const float TAU = 2.0 * PI;
	ivec2 px = ivec2(gl_FragCoord.xy);

	//(Usher, 2015)
	float phi = (30u * px.x ^ px.y + 10u * px.x * px.y);
	
	for (int i = 0; i < samples; ++i)
	{
		// Define Alchemy AO helper variables (McGuire, Mara and Luebke, 2012)
		float alpha = 1.f / samples * (i + 0.5);
		float h = screen_radius * alpha;
		float theta = TAU * alpha * turns + phi;
		vec2 u = vec2(cos(theta), sin(theta));

		// McGure MIP calculation (McGuire, Mara and Luebke, 2012)
		int m = clamp(findMSB(int(h)) - 4, 0, max_mip);
		ivec2 mip_pos = clamp((ivec2(h * u) + px) >> m, ivec2(0), textureSize(gPosition, m) - ivec2(1));

		//(Usher, 2015)
		//vec3 worldPi = texelFetch(gPosition, mip_pos, m).xyz;
		vec3 Pi = texelFetch(gPosition, mip_pos, m).xyz;
		//vec3 Pi = vec3(camera.view * vec4(worldPi, 1.0));
		vec3 V = Pi - Position;
		float sqrLen    = dot(V, V);
		float Heaviside = step(length(sqrLen), sampleRadius);
		float dD = depthThreshold * Position.z;
		float c = 0.1 * sampleRadius;
								  
		ambientValue += (max(0.0, dot(Normal, V) + dD) * Heaviside) / (max(c * c, sqrLen) + epsilon);
	  
	}
	
	ambientValue *= (2.0 * shadowScalar) / samples;
	ambientValue = max(0.0, 1.0 - pow(ambientValue, shadowContrast));
	
    outColor = vec4(ambientValue, ambientValue, ambientValue, 1.0);
}


