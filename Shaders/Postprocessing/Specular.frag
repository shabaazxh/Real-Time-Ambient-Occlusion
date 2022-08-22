#version 450

layout(binding = 0) uniform SSAOubo {
	vec4 samples[64];
	int sample_amount;
	float radius;

	int hbaoSampleDirection;
	float hbaoSteps;
	int hbaoNumberOfSteps;
	float hbaoAmbientLightLevel;

	float alchemySigma;
	float alchemyKappa;

	float time;
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

#define MAX_SHININESS 127.75

void main()
{
	outColor = vec4(vec3(ssao.radius, ssao.radius, ssao.radius), clamp(75.0 / MAX_SHININESS, 0, 1));
}