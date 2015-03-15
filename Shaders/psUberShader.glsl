
#version 400


in vec2 vs_outTex;
in vec3 vs_outNormal;
in vec3 vs_outPosition;

out vec4 outColor;

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

//---------------------------------------------------------------------------------------
// Other Uniforms
//---------------------------------------------------------------------------------------
uniform vec3		camPosition;
uniform vec3		LightPosition;		// light position  
uniform vec3		Kd;					// Diffuse reflectivity
uniform vec3		Ld;					// light source intensity

uniform sampler2D	texture_diffuse1;
uniform sampler2D	texture_specular1;

//---------------------------------------------------------------------------------------
// Phong Specular
//---------------------------------------------------------------------------------------
vec4 PhongBRDF(vec3 normal, vec3 camLook, vec3 reflection)
{
	float VdotR = max(dot(camLook, reflection), 0);
	float specular = pow(VdotR, 64);
	
	return vec4(specular, specular, specular, 1.0f);
}

//---------------------------------------------------------------------------------------
// Blinn Specular
//---------------------------------------------------------------------------------------
vec4 BlinnBRDF(vec3 normal, vec3 camLook, vec3 half)
{
	float NdotH = clamp(dot(normal, half), 0, 1);
	float specular = pow(NdotH, 64);
	
	return vec4(specular, specular, specular, 1.0f);
}

//---------------------------------------------------------------------------------------
// Cook Torrance Specular
//---------------------------------------------------------------------------------------
vec4 CookTorranceBRDF(vec3 normal, vec3 camLook, vec3 lightDir, vec3 half)
{
	float roughness = 0.1f;

	float NdotL = clamp(dot(normal, lightDir), 0, 1);
	float NdotH = clamp(dot(normal, half), 0, 1);
	float NdotV = clamp(dot(normal, camLook), 0, 1);
	float VdotH = clamp(dot(camLook, half), 0, 1);
	float r_sq = roughness * roughness;

	// Evaluate the geometric term
	//-------------------------------
	float geo_numerator = 2 * NdotH;
	float geo_denominator = VdotH;

	float geo_b = (geo_numerator * NdotV ) / geo_denominator;
    float geo_c = (geo_numerator * NdotL ) / geo_denominator;
    float geo   = min( 1.0f, min( geo_b, geo_c ) );

	// Now evaluate the roughness term
    // -------------------------------
	float roughness_a = 1.0f / ( 4.0f * r_sq * pow( NdotH, 4 ) );
    float roughness_b = NdotH * NdotH - 1.0f;
    float roughness_c = r_sq * NdotH * NdotH;
 
    float roughness_value = roughness_a * exp( roughness_b / roughness_c );

	// Next evaluate the Fresnel value
	// http://filmicgames.com/archives/557
    // -------------------------------
    float base = 1 - VdotH;
	float exponential = pow( base, 5.0);
	float F0 = 0.1f;
	float fresnel = exponential + F0 * (1.0 - exponential);
	
	// Put all the terms together to compute
    // the specular term in the equation
    // -------------------------------------
	float Rs_Nr = fresnel * geo * roughness_value;
    vec3  Rs_numerator    = vec3( Rs_Nr, Rs_Nr, Rs_Nr );
    float Rs_denominator  = NdotV * NdotL;
    vec3  Rs              = Rs_numerator / Rs_denominator;

	vec4 finalSpec = vec4(Rs, 1.0f);

	return finalSpec;
}

//---------------------------------------------------------------------------------------
void main()
{
	vec4 baseColor = vec4(texture(texture_diffuse1, vs_outTex));		// base Map color
	vec4 specColor = vec4(texture(texture_specular1, vs_outTex));	// specular map color

	// calculate camera eye vector in world space
	vec3 Eye = normalize(vs_outPosition - camPosition);

	// calculate view vector
	vec3 view = -Eye;

	// ------------------------ Directional Illuminance -------------------
	vec4 DiffuseDir = vec4(0,0,0,1);
	vec4 SpecularDir = vec4(0,0,0,1);
	vec3 halfDir = vec3(0,0,0);
	float NdotLDir = 0.0f;
	float SpecDir = 0.0f;

	for(int i = 0 ; i < numDirLights ; ++i)
	{
		// diffuse
		NdotLDir = max(dot(vs_outNormal, -dirLights[i].direction), 0);

		// specular
		halfDir = normalize(-dirLights[i].direction + view);

		if(NdotLDir > 0)
			SpecDir = BlinnBRDF(vs_outNormal, view, halfDir);

		// accumulate...
		DiffuseDir += dirLights[i].color * NdotLDir * dirLights[i].intensity;
		SpecularDir += dirLights[i].color * SpecDir * dirLights[i].intensity;
	}

	// ------------------------ Point Light Illuminance ------------------- 
	// Diffuse & Specular accumulators for Point lights
	vec4 DiffusePoint = vec4(0,0,0,1);
	vec4 SpecularPoint = vec4(0,0,0,1);
	vec3 LightDir = vec3(0,0,0);
	vec3 halfPoint = vec3(0,0,0);
	float NdotLPoint = 0.0f;
	float SpecPoint = 0.0f;
	float atten = 0.0f;

	//--- Point Light contribution 
	for(int i = 0 ; i < numPointLights ; ++i)
	{
		LightDir = normalize(vs_outPosition - pointLights[i].position);
		float dist = length(LightDir);
		float r = pointLights[i].radius;

		// ref : https://imdoingitwrong.wordpress.com/2011/01/31/light-attenuation/
		atten = 1 / (1 + ((2/r)*dist) + ((1/r*r)*(dist*dist)));
		
		// diffuse
		NdotLPoint = max(dot(vs_outNormal, -LightDir), 0);

		// specular
		halfPoint = normalize(-LightDir + view);

		if(NdotLPoint > 0)
			SpecPoint = BlinnBRDF(vs_outNormal, view, halfPoint);

		// accumulate...
		DiffusePoint += pointLights[i].color * atten * NdotLPoint * pointLights[i].intensity;
		SpecularPoint += pointLights[i].color * atten * SpecPoint * pointLights[i].intensity;
	}

	// Final Color components...
	vec4 Emissive			= baseColor;
	vec4 Ambient			= vec4(0.4, 0.4, 0.4, 1);
	vec4 DiffuseDirect		= DiffuseDir + DiffusePoint;
	vec4 SpecularDirect		= SpecularDir + SpecularPoint; 

	outColor = Emissive * (Ambient + DiffuseDirect) + specColor * SpecularDirect; 
	
	// linear depth
	/*float near = 0.1f;
	float far = 100.0f;

	float z = gl_FragCoord.z * 2 - 1;
	float depth = (2 * near) / (far + near - z * (far-near));

	outColor = vec4(vec3(1- depth), 1);*/
}