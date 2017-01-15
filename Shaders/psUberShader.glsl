
#version 400

#define PI 3.141592653589793

in vec2		vs_outTex;
in vec3		vs_outNormal;
in vec3		vs_outTangent;
in vec3		vs_outBinormal;
in vec3		vs_outPosition;

layout (location = 0) out vec4 positionColor;
layout (location = 1) out vec4 normalColor;
layout (location = 2) out vec4 albedoColor; 
layout (location = 3) out vec4 reflectionColor;
layout (location = 4) out vec4 emissiveColor;
layout (location = 5) out vec4 backgroundColor;

//---------------------------------------------------------------------------------------
// Material Properties
//---------------------------------------------------------------------------------------
struct Material
{
	vec4 Albedo;
	vec4 Emissive;
	float Roughness;
	float Metallic;
};

uniform Material material;

//---------------------------------------------------------------------------------------
// Other Uniforms
//---------------------------------------------------------------------------------------
uniform sampler2D	texture_diffuse;
uniform sampler2D	texture_specular;
uniform sampler2D	texture_normal;	
uniform sampler2D   texture_ambient;
uniform sampler2D	texture_emissive;	
uniform sampler2D	texture_height;
uniform sampler2D	texture_environment;   

uniform vec3 camPosition;

uniform bool bDiffuseTexture;
uniform bool bSpecularTexture;
uniform bool bNormalMapTexture;
uniform bool bEmissiveTexture;
uniform bool bHeightMapTexture;
uniform bool bAmbientOccTexture;
uniform bool bShininessTexture;
uniform bool bDisplacementTexture;
uniform bool bLightMapTexture;
uniform bool bReflectionTexture;


vec2 RadialCoords(vec3 a_coords)
{
    vec3 a_coords_n = normalize(a_coords);
    float lon = atan(a_coords_n.z, a_coords_n.x);
    float lat = acos(a_coords_n.y);
    vec2 sphereCoords = vec2(lon, lat) * (1.0 / PI);
    return vec2(sphereCoords.x * 0.5 + 0.5, 1 - sphereCoords.y);
}

//---------------------------------------------------------------------------------------
void main()
{
	// Final contributors...
	vec4 Albedo;
	vec4 Specular;
	vec4 Emissive;
	vec4 Normal;
	vec4 Reflection;
	vec4 Background;
	float ao;

	// BaseMap color aka Albedo
	if(bDiffuseTexture)
		Albedo = texture(texture_diffuse, vs_outTex) * material.Albedo;
	else
		Albedo = material.Albedo;

	// Specular Color
	if(bSpecularTexture)
		Specular = texture(texture_specular, vs_outTex);

	// Emissive Color
	if(bEmissiveTexture)
		Emissive = texture(texture_emissive, vs_outTex) * material.Emissive;
	else
		Emissive = material.Emissive;

	// Ambient occlusion gets stored in alpha channel of albedo buffer
	if(bAmbientOccTexture)
		ao = texture(texture_ambient, vs_outTex).r;
	else
		ao = 0.0f;

	// Normal map
	if(bNormalMapTexture)
		Normal = texture(texture_normal, vs_outTex) * 2.0 - 1.0f;
	else
		Normal.xyz = normalize(vs_outNormal);

	// View vector
	vec3 view = normalize(camPosition - vs_outPosition);

	// calculate reflection vector for environment mapping..
	vec3 R = reflect(view, normalize(vs_outNormal));
	vec2 equiUV = -RadialCoords(R);
	Reflection = textureLod(texture_environment, equiUV, material.Roughness * 10.0f);
	Reflection.a = material.Roughness;

	//vec4 ambientColor = vec4(textureLod(texture_cubeMap, vs_outNormal, 7)); 

	// Buffer color outputs for deferred rendering
	positionColor = vec4(vs_outPosition, 1.0f);		// world space position data
	normalColor.rgb = Normal.xyz;					// normal data
	normalColor.a = Specular.r;						// specular data
	albedoColor.rgb = Albedo.rgb;					// albedo color
	albedoColor.a = ao;								// ambient occlusion data
	reflectionColor.rgb = Reflection.rgb;			// Reflection data
	reflectionColor.a = material.Roughness;			// Roughness data
	emissiveColor.rgb = Emissive.rgb;				// Emissive color
	emissiveColor.a  = material.Metallic;			// Metallic data


	// = Emissive * Diffuse + 0.35*reflectionColor; 

	// check whether fragment color is more than the threshold brightness value
	// we calculate first grayscale equivalent...
	/*float brightness = dot(outColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
	if(brightness > 1.0f)
		brightColor = vec4(outColor.rgb, 1.0f);*/
	
	// linear depth
	/*float near = 0.1f;
	float far = 10.0f;

	float z = gl_FragCoord.z * 2 - 1;
	float depth = (2 * near) / (far + near - z * (far-near));

	outColor = vec4(vec3(1- depth), 1);*/
}