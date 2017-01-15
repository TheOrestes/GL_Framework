
#version 400

#define PI 3.1428571

in vec2 vs_outTexCoord;

layout (location = 0) out vec4 screenColor;
layout (location = 1) out vec4 brightColor;

//out vec4 outColor;

uniform sampler2D positionBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D albedoBuffer;
uniform sampler2D reflectionBuffer;
uniform sampler2D emissiveBuffer;
uniform sampler2D backgroundBuffer;
uniform float bloomThreshold;

//---------------------------------------------------------------------------------------
// Point Lights
//---------------------------------------------------------------------------------------
#define MAX_POINT_LIGHTS 8

uniform int			numPointLights;		// number of point lights in the scene
struct PointLight
{
	float radius;
	float intensity;
	vec3 position;
	vec3 color;
};

uniform PointLight pointLights[MAX_POINT_LIGHTS];

//---------------------------------------------------------------------------------------
// Directional Lights
//---------------------------------------------------------------------------------------
#define MAX_DIR_LIGHTS 8

uniform int			numDirLights;		// number of point lights in the scene
struct DirectionalLight
{
	float intensity;
	vec3 direction;
	vec3 color;
};

uniform DirectionalLight dirLights[MAX_DIR_LIGHTS];
uniform vec3 camPosition;

//---------------------------------------------------------------------------------------
// Blinn Specular
//---------------------------------------------------------------------------------------
vec4 BlinnBRDF(vec3 normal, vec3 half, float roughness)
{
	float NdotH = clamp(dot(normal, half), 0, 1);
	float specular = pow(NdotH, 1/roughness);

	//float specular = pow(NdotH, 64);
	
	return vec4(specular, specular, specular, 1.0f);
}

//---------------------------------------------------------------------------------------
// Cook Torrance Specular
//---------------------------------------------------------------------------------------
float D_GGX_TR(vec3 N, vec3 H, float r_sq)
{
	float NdotH = clamp(dot(N, H), 0, 1);
	float NdotH_sq = NdotH * NdotH;

	float denom = ((NdotH_sq) * (r_sq-1) + 1);
	denom = denom * denom * PI;

	return (r_sq / denom);
}

float GeometrySchlickGGX(float NdotV, float k)
{
	return (NdotV /  (NdotV)*(1-k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
	float NdotV = clamp(dot(N, V), 0, 1);
	float NdotL = clamp(dot(N, L), 0, 1);
	float ggx1 = GeometrySchlickGGX(NdotV, k);
	float ggx2 = GeometrySchlickGGX(NdotL, k);

	return ggx1*ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0, float roughness)
{
	//return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

//---------------------------------------------------------------------------------------
// Main Function
//---------------------------------------------------------------------------------------
void main()
{
	// Final contributors...
	vec4 Albedo = vec4(0);
	vec4 Ambient = vec4(0);
	vec4 Reflection = vec4(0);
	vec4 Background = vec4(0);
	vec4 Emissive = vec4(0);

	// Input from G-Buffers
	vec4 positionColor = texture(positionBuffer, vs_outTexCoord);
	vec4 normalColor = texture(normalBuffer, vs_outTexCoord);
	vec4 albedoColor = texture(albedoBuffer, vs_outTexCoord);
	vec4 reflectionColor = texture(reflectionBuffer, vs_outTexCoord);
	vec4 emissiveColor = texture(emissiveBuffer, vs_outTexCoord);
	vec4 backgroundColor = texture(backgroundBuffer, vs_outTexCoord);
	float ao = albedoColor.a; 
	float specColor = normalColor.a;

	vec3 N = normalize(normalColor.xyz);
	vec3 V = normalize(camPosition - positionColor.rgb);

	float roughness = reflectionColor.a;
	float metallic = emissiveColor.a;
	float r_sq = roughness * roughness;

	float NdotV = clamp(dot(N, V), 0, 1);

	// Fresnel 
	vec3 F0 = vec3(0.03, 0.03, 0.03);
	F0 = mix(F0, albedoColor.rgb, metallic);
	vec3 F = fresnelSchlick(NdotV, F0, roughness);
	vec3 Ks = F;
	vec3 Kd = vec3(1.0) - Ks;
	Kd *= 1.0 - metallic;

	// Albedo
	Albedo			= albedoColor;//vec4(0.0, 0, 0.6, 1.0); 

	if(ao > 0)
		Ambient			= Albedo * vec4(vec3(ao),1);
	else
		Ambient			= Albedo;

	// ------------------------ Directional Illuminance -------------------
	vec3 LoDir = vec3(0);

	for(int i = 0 ; i < numDirLights ; ++i)
	{
		vec3 lightDir = normalize(-dirLights[i].direction);
		vec3 radianceDir = dirLights[i].color * dirLights[i].intensity;
		vec3 halfDir = normalize(lightDir + V);

		// diffuse
		float NdotLDir = clamp(dot(N, lightDir), 0, 1);

		// cook torrance brdf
		float D = D_GGX_TR(N, halfDir, r_sq);
		float G = GeometrySmith(N, V, lightDir, roughness);
		vec3 nominator = D * G * F;
		float denominator = 4 * NdotV * NdotLDir;
		vec3 brdfDir = nominator / denominator;		

		LoDir += (Kd * albedoColor.rgb / PI + brdfDir) * radianceDir * NdotLDir;
	}

	// ------------------------ Point Light Illuminance ------------------- 
	vec3 LoPoint = vec3(0);

	for(int i = 0 ; i < numPointLights ; ++i)
	{
		vec3 direction = -normalize(positionColor.rgb - pointLights[i].position);
		vec3 halfPoint = normalize(direction + V);
		float dist = length(direction);
		float atten = 1.0 / (dist*dist);
		vec3 radiancePoint = pointLights[i].color * pointLights[i].intensity * atten;

		// diffuse
		float NdotLPoint = clamp(dot(N, direction), 0, 1);
		
		// cook torrance brdf
		float D = D_GGX_TR(N, halfPoint, r_sq);
		float G = GeometrySmith(N, V, direction, roughness);
		vec3 nominator = D * G * F;
		float denominator = 4 * NdotV * NdotLPoint + 0.001;
		vec3 brdfPoint = nominator / denominator;	

		LoPoint += (Kd * albedoColor.rgb / PI + brdfPoint) * radiancePoint * NdotLPoint;
	}

	vec3 Lo = LoDir + LoPoint;

	//vec4 Color = Ambient + vec4(Lo,1);

	Reflection		= reflectionColor;
	Emissive		= emissiveColor;
	Background 		= backgroundColor;
	
	screenColor = Emissive + Ambient * (vec4(Lo,1));

	// calculate additional brightness from overall scene...
	float brightness = dot(screenColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
	if(brightness > bloomThreshold)
		brightColor = vec4(screenColor.rgb, 1.0f);
}

