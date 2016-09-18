
#version 400

#define PI 3.1428571

in vec2		vs_outTex;
in vec3		vs_outNormal;
in vec3		vs_outTangent;
in vec3		vs_outBinormal;
in vec3		vs_outPosition;

layout (location = 0) out vec4 positionColor;
layout (location = 1) out vec4 normalColor;
layout (location = 2) out vec4 albedoColor; 
layout (location = 3) out vec4 cubemapColor;
layout (location = 4) out vec4 emissiveColor;

//---------------------------------------------------------------------------------------
// Material Properties
//---------------------------------------------------------------------------------------
struct Material
{
	vec4 Albedo;
	vec4 Emissive;
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
uniform samplerCube texture_cubeMap;   

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

//---------------------------------------------------------------------------------------
// Phong Specular
//---------------------------------------------------------------------------------------
vec4 PhongBRDF(vec3 camLook, vec3 reflection)
{
	float VdotR = max(dot(camLook, reflection), 0);
	float specular = pow(VdotR, 32);
	
	return vec4(specular, specular, specular, 1.0f);
}

//---------------------------------------------------------------------------------------
// Blinn Specular
//---------------------------------------------------------------------------------------
vec4 BlinnBRDF(vec3 normal, vec3 half)
{
	float NdotH = max(dot(normal, half), 0);
	//float specular = pow(NdotH, 1/(1-material.Roughness.x));

	float specular = pow(NdotH, 32);
	
	return vec4(specular, specular, specular, 1.0f);
}

//---------------------------------------------------------------------------------------
// Cook Torrance Specular
//---------------------------------------------------------------------------------------
vec4 CookTorranceBRDF(vec3 normal, vec3 camLook, vec3 lightDir, vec3 half)
{
	float roughness = 0.5f;// material.Roughness.x;

	float NdotL = clamp(dot(normal, lightDir), 0, 1);
	float NdotH = clamp(dot(normal, half), 0, 1);
	float NdotV = clamp(dot(normal, camLook), 0, 1);
	float VdotH = clamp(dot(camLook, half), 0, 1);
	float r_sq = roughness * roughness;

	// Microfacet distribution function
	//-------------------------------
	float nh = length(normal) * length(half);
	float beta = acos(NdotH / nh);
	float Nr = exp(-pow((tan(beta) / roughness), 2));
	float Dr = 4 * r_sq * pow(cos(beta), 4);
	float D = Nr / Dr;

	// Evaluate the geometric term
	//-------------------------------
	float geo_numerator = 2 * NdotH;
	float geo_denominator = VdotH;

	float geo_b = (geo_numerator * NdotV ) / geo_denominator;
    float geo_c = (geo_numerator * NdotL ) / geo_denominator;
    float geo   = min( 1.0f, min( geo_b, geo_c ) );


	// Now evaluate the roughness term
    // -------------------------------
	/*float roughness_a = 1.0f / ( 4.0f * r_sq * pow( NdotH, 4 ) );
    float roughness_b = NdotH * NdotH - 1.0f;
    float roughness_c = r_sq * NdotH * NdotH;
 
    float roughness_value = roughness_a * exp( roughness_b / roughness_c );*/

	// Next evaluate the Fresnel value
	// http://filmicgames.com/archives/557
    // -------------------------------
    float base = 1 - VdotH;
	float exponential = pow( base, 5.0);
	// Natty Hoffman : SIGGRAPH 2010 ( Physically based shading ) 
	// F0 depends on material we are planning to use
	// Material				F(0◦) (Linear)		F(0◦) (sRGB) 
	// Water				0.02,0.02,0.02		0.15,0.15,0.15
	// Plastic / Glass(Low)	0.03,0.03,0.03		0.21,0.21,0.21
	// Plastic High			0.05,0.05,0.05		0.24,0.24,0.24
	// Glass (High)/Ruby	0.08,0.08,0.08		0.31,0.31,0.31
	// Diamond				0.17,0.17,0.17		0.45,0.45,0.45
	// Iron					0.56,0.57,0.58		0.77,0.78,0.78
	// Copper				0.95,0.64,0.54		0.98,0.82,0.76
	// Gold					1.00,0.71,0.29		1.00,0.86,0.57
	// Aluminum				0.91,0.92,0.92		0.96,0.96,0.97
	// Silver				0.95,0.93,0.88		0.98,0.97,0.95
	vec3 F0 = vec3(0.03, 0.03, 0.03);
	vec3 FSchlick = F0 + exponential * (1-F0); 
	
	// Put all the terms together to compute
    // the specular term in the equation
    // -------------------------------------
	vec3 Rs_Nr = FSchlick * geo * D;
    vec3  Rs_numerator    = Rs_Nr;
    float Rs_denominator  = NdotV * NdotL;
    vec3  Rs              = Rs_numerator / Rs_denominator;

	vec4 finalSpec = vec4(Rs, 1.0f);

	return finalSpec;
}

//---------------------------------------------------------------------------------------
void main()
{
	// Final contributors...
	vec4 Albedo;
	vec4 Specular;
	vec4 Emissive;
	vec4 Normal;
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
	vec4 reflectionColor = texture(texture_cubeMap, R);

	//vec4 ambientColor = vec4(textureLod(texture_cubeMap, vs_outNormal, 7)); 

	// Buffer color outputs for deferred rendering
	positionColor = vec4(vs_outPosition, 1.0f);		// world space position data
	normalColor.rgb = vs_outNormal;					// normal data
	normalColor.a = Specular.r;						// specular data
	albedoColor.rgb = Albedo.rgb;					// albedo color
	albedoColor.a = ao;								// ambient occlusion data
	cubemapColor = reflectionColor;					// reflection data
	emissiveColor = Emissive;						// Emissive color


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