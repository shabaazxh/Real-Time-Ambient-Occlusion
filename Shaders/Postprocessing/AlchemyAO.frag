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

float sampleRadius = ssao.radius;
int samples = ssao.sample_amount; 
float bias = 0.001;
float shadowScalar = ssao.alchemySigma; 
float shadowContrast = ssao.alchemyKappa;
const float epsilon = 0.0001;

//(ZaOniRinku, 2021) Use depth to obtain position data
vec3 depthToPosition(vec2 tc)
{
	float depth = texture(depthMap, tc).x;
	vec4 clipSpace = vec4(tc * 2.0 - 1.0, depth, 1.0);
	vec4 viewSpace = inverse(camera.proj) * clipSpace;
	return viewSpace.xyz / viewSpace.w;
}

//(ZaOniRinku, 2021) Use depth to obtain normal data
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

float RANDOMVAUE = 0.0f;

//https://stackoverflow.com/a/4275343/14723580
// Generate random values 
vec2 RandomHashValue()
{
	return fract(sin(vec2(RANDOMVAUE += 0.1, RANDOMVAUE += 0.1)) * vec2(43758.5453123, 22578.1459123));
}

//https://stackoverflow.com/a/50746409/14723580
// Obtain a random sample from the disk, returned as cartesian coordinates
vec2 DiskPoint(float sampleRadius, float x, float y)
{
	float r = sampleRadius * sqrt(x); // square root number to map point to radius
	float theta = y * (2.0 * PI); // theta is angle in radians inside disk
	return vec2(r * cos(theta), r * sin(theta));
}

//Sigma is intensity
//Kappa is contrast

void main()
{
	// Randon value updating based on (Rasquinha, 2022) random hash implementation
	RANDOMVAUE = (uvCoords.x * uvCoords.y) * 64.0;
	RANDOMVAUE += fract(ssao.time) * 64.0f;
	
	// Normals and positions in view-space
	vec3 Position = depthToPosition(uvCoords);
	vec3 Normal = depthToNormal(uvCoords).xyz;

	float ao = 0.0;
	//(Timurson, 2021) projection of a ball around the point 
	float screen_radius = (sampleRadius * 0.75 / Position.z); //ball around the point 
	
	for (int i = 0; i < samples; ++i)
	{
		vec2 RandomValue = RandomHashValue(); // Generate a random value
		vec2 disk = DiskPoint(sampleRadius, RandomValue.x, RandomValue.y); // Obtain disk point of random value 
		vec2 samplepos = uvCoords + (disk.xy) * screen_radius; // offset disk point from current uv 

		//(Timurson, 2021) built understanding for final calculations from his different AO method implementation
		vec3 P = depthToPosition(samplepos.xy); // convert sample to view-space
		vec3 V = P - Position; //calculate distance from current frag position to sample position
		float Heaviside = step(length(dot(V, V)), sampleRadius); //Ensure it is within the radius of influence using step
		float dD = bias * Position.z; //multiply bias by z as suggested by paper, to prevent dot products becoming sensitive  
								  
		// Apply summations calculations following equation 10 from paper  
		ao += (max(0.0, dot(Normal, V) + dD) * Heaviside) / (dot(V, V) + epsilon);
	}
	
	// Apply final calculations for AO following equation 10 from paper 
	ao *= (2.0 * shadowScalar) / samples; //apply shadow scalar as show in the equation of the paper
	ao = max(0.0, 1.0 - pow(ao, shadowContrast)); //final ambient value

    outColor = vec4(ao, ao, ao, 1.0);
}


