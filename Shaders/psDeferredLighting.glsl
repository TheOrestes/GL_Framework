
#version 400

#define PI 3.1428571

in vec2 vs_outTexCoord;

out vec4 outColor;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D albedoTexture;
uniform sampler2D cubemapTexture;

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
	vec4 color;
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
	vec4 color;
};

uniform DirectionalLight dirLights[MAX_DIR_LIGHTS];
uniform vec3 camPosition;

//---------------------------------------------------------------------------------------
// Blinn Specular
//---------------------------------------------------------------------------------------
vec4 BlinnBRDF(vec3 normal, vec3 half)
{
	float NdotH = max(dot(normal, half), 0);
	//float specular = pow(NdotH, 1/(1-material.Roughness.x));

	float specular = pow(NdotH, 16);
	
	return vec4(specular, specular, specular, 1.0f);
}

//---------------------------------------------------------------------------------------
// Main Function
//---------------------------------------------------------------------------------------
void main()
{
	// Final contributors...
	vec4 Emissive = vec4(0);
	vec4 Ambient = vec4(0);
	vec4 Diffuse = vec4(0);
	vec4 Specular = vec4(0);
	vec4 Reflection = vec4(0);

	// Input from G-Buffers
	vec3 positionColor = texture(positionTexture, vs_outTexCoord).rgb;
	vec4 normalColor = texture(normalTexture, vs_outTexCoord);
	vec4 albedoColor = texture(albedoTexture, vs_outTexCoord);
	vec4 cubemapColor = texture(cubemapTexture, vs_outTexCoord);
	float ao = albedoColor.a; 
	float specColor = normalColor.a;

	// View vector
	vec3 view = normalize(camPosition - positionColor);

	// ------------------------ Directional Illuminance -------------------
	vec4 DiffuseDir = vec4(0,0,0,1);
	float NdotLDir = 0.0f;
	vec4 SpecularDir = vec4(0,0,0,1);
	vec3 halfDir = vec3(0,0,0);
	vec4 SpecDir = vec4(0,0,0,1);

	for(int i = 0 ; i < numDirLights ; ++i)
	{
		vec3 lightDir = normalize(-dirLights[i].direction);

		// diffuse
		NdotLDir = max(dot(normalColor.xyz, lightDir), 0);

		// specular
		halfDir = normalize(lightDir + view);
			
		SpecDir = BlinnBRDF(normalColor.xyz, halfDir) * NdotLDir;

		// accumulate...
		DiffuseDir += dirLights[i].color * NdotLDir * dirLights[i].intensity;
		SpecularDir += dirLights[i].color * SpecDir * dirLights[i].intensity;
	}

	// ------------------------ Point Light Illuminance ------------------- 
	// Diffuse & Specular accumulators for Point lights
	vec4 DiffusePoint = vec4(0,0,0,1);
	vec3 LightDir = vec3(0,0,0);
	float NdotLPoint = 0.0f;
	float atten = 0.0f;
	vec4 SpecularPoint = vec4(0,0,0,1);
	vec3 halfPoint = vec3(0,0,0);
	vec4 SpecPoint = vec4(0,0,0,1);
	

	//--- Point Light contribution 
	for(int i = 0 ; i < numPointLights ; ++i)
	{
		LightDir = normalize(positionColor - pointLights[i].position);
		float dist = length(LightDir);
		float r = pointLights[i].radius;

		// ref : https://imdoingitwrong.wordpress.com/2011/01/31/light-attenuation/
		atten = 1 / dist; //(1 + ((2/r)*dist) + ((1/r*r)*(dist*dist)));
		
		// diffuse
		NdotLPoint = max(dot(normalColor.xyz, -LightDir), 0);

		// specular
		halfPoint = normalize(-LightDir + view);

		SpecPoint = BlinnBRDF(normalColor.xyz, halfPoint) * NdotLPoint;

		// accumulate...
		DiffusePoint += pointLights[i].color * atten * NdotLPoint * pointLights[i].intensity;
		SpecularPoint += pointLights[i].color * atten * SpecPoint * pointLights[i].intensity;
	}

	// Final Color components...
	Emissive		= vec4(albedoColor.xyz,1);//vec4(0.0, 0, 0.6, 1.0); 
	Ambient			= vec4(vec3(ao),1);
	Diffuse			= DiffuseDir + DiffusePoint;
	Specular		= SpecularDir + SpecularPoint; 
	Reflection		= cubemapColor;
	
	outColor = Emissive * (Ambient + Diffuse/PI + Specular + specColor*Reflection); //(Ambient + specColor * Specular); //Emissive * (Ambient + Diffuse + Specular);
	
	// Gamma correction!
	outColor = vec4(pow(outColor.xyz, vec3(1/2.2)), 1);
}

