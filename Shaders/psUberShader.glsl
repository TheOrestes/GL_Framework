
#version 400

#define PI 3.1428571

in vec2		vs_outTex;
in vec3		vs_outNormal;
in vec3		vs_outTangent;
in vec3		vs_outBinormal;
in vec3		vs_outPosition;

out vec4	outColor;

//---------------------------------------------------------------------------------------
// Material Properties
//---------------------------------------------------------------------------------------
struct Material
{
	vec4 Color;
	vec4 specularColor;
	vec4 roughnessColor;
	vec4 reflectionColor;

	float metallic;
};

uniform Material material;

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
uniform mat4		matWorld;
uniform mat4		matWorldInv;

uniform sampler2D	texture_diffuse1;
uniform sampler2D	texture_specular1;
uniform sampler2D	texture_normal1;		
uniform samplerCube texture_cubeMap;   
uniform bool		bNormalMapping = true;

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
	//float specular = pow(NdotH, 1/(1-material.roughnessColor.x));

	float specular = pow(NdotH, 128);
	
	return vec4(specular, specular, specular, 1.0f);
}

//---------------------------------------------------------------------------------------
// Cook Torrance Specular
//---------------------------------------------------------------------------------------
vec4 CookTorranceBRDF(vec3 normal, vec3 camLook, vec3 lightDir, vec3 half)
{
	float roughness = material.roughnessColor.x;

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
	vec4 Diffuse = vec4(0);
	vec4 Specular = vec4(0);
	vec4 Emissive = vec4(0);
	vec4 Ambient = vec4(0);
	vec4 Reflection = vec4(0);
	vec3 shadingNormal = vec3(0);
	vec3 envMapNormal = vec3(0);
	vec3 objSpaceNormal = vec3(0);

	// calculate TBN matrix
	mat3 TBN = mat3(vs_outTangent,vs_outBinormal,vs_outNormal);
	mat3 TangentToObject = TBN;
	mat3 ObjectToTangent = transpose(TBN);

	// BaseMap color aka Albedo
	vec4 baseMapColor = vec4(texture(texture_diffuse1, vs_outTex));
	// Specular Map color
	vec4 specMapColor = vec4(texture(texture_specular1, vs_outTex));
	// Normal map
	vec3 normalMap = texture(texture_normal1, vs_outTex).rgb;

	if(bNormalMapping)
	{
		shadingNormal = normalize(normalMap * 2.0 - 1.0);				// tangent space normal!
		envMapNormal = normalize(TangentToObject * shadingNormal);	
		//envMapNormal = normalize((matWorld * vec4(envMapNormal, 0)).xyz);
		//objSpaceNormal = normalize(inverse(matWorld) * vec4(envMapNormal,1)).xyz;

	}
	else
	{
		shadingNormal = normalize(vs_outNormal);	// object space normal!
		//envMapNormal = shadingNormal;
	}

	// calculate camera eye vector in world space
	vec3 Eye = normalize(vs_outPosition - camPosition);

	// calculate object space view vector
	vec3 view = vec3(0,0,0);
	vec3 ObjectSpaceView = normalize((matWorldInv * vec4(-Eye, 0)).xyz);

	if(bNormalMapping)
	{
		view = ObjectToTangent * ObjectSpaceView;
	}
	else
	{
		view = -ObjectSpaceView;		// Object Space!
	}
	

	// ------------------------ Directional Illuminance -------------------
	vec3 LightDir = vec3(0,0,0);
	vec4 DiffuseDir = vec4(0,0,0,1);
	vec4 SpecularDir = vec4(0,0,0,1);
	vec3 halfDir = vec3(0,0,0);
	vec3 lightRefl = vec3(0,0,0);
	float NdotLDir = 0.0f;
	vec4 SpecDir = vec4(0,0,0,1);

	vec3 ObjectSpaceLightDir = vec3(0,0,0);

	for(int i = 0 ; i < numDirLights ; ++i)
	{
		ObjectSpaceLightDir = normalize((matWorldInv * vec4(dirLights[i].direction, 0)).xyz);
		// diffuse
		if(bNormalMapping)
		{
			LightDir = ObjectToTangent * ObjectSpaceLightDir;
		}
		else
		{
			LightDir = ObjectSpaceLightDir;
		}
		

		NdotLDir = max(dot(shadingNormal, -LightDir), 0);

		// phong specular
		lightRefl = normalize(reflect(LightDir, shadingNormal));
		
		// blinn specular
		halfDir = normalize(-LightDir + view);

		// Instead of an If condition, it is sensible to mulitply specular term with cosine term
		// Ref : Natty Hoffman : SIGGRAPH 2010 ( Physically based shading ) 
		//if(NdotLDir > 0)
		//SpecDir = PhongBRDF(view, lightRefl);
		//SpecDir = CookTorranceBRDF(shadingNormal, view, -dirLights[i].direction, halfDir) * NdotLDir;  // 
		SpecDir = BlinnBRDF(shadingNormal, halfDir) * NdotLDir;
		
		// accumulate...
		DiffuseDir += dirLights[i].color * NdotLDir * dirLights[i].intensity;
		SpecularDir += dirLights[i].color * SpecDir * dirLights[i].intensity;
	}

	// ------------------------ Point Light Illuminance ------------------- 
	// Diffuse & Specular accumulators for Point lights
	vec4 DiffusePoint = vec4(0,0,0,1);
	vec4 SpecularPoint = vec4(0,0,0,1);
	vec3 halfPoint = vec3(0,0,0);
	float NdotLPoint = 0.0f;
	vec4 SpecPoint = vec4(0,0,0,1);
	float atten = 0.0f;

	//--- Point Light contribution 
	for(int i = 0 ; i < numPointLights ; ++i)
	{
		// Object space light position
		vec3 ObjectSpacePosition = normalize((matWorldInv * vec4(pointLights[i].position, 0)).xyz);

		if(bNormalMapping)
		{
			LightDir = ObjectToTangent * normalize(vs_outPosition - ObjectSpacePosition);
		}
		else
		{
			LightDir = normalize(vs_outPosition - ObjectSpacePosition);
		}
		
		float dist = length(LightDir);
		float r = pointLights[i].radius;

		// ref : https://imdoingitwrong.wordpress.com/2011/01/31/light-attenuation/
		atten = 1/dist; //1 / (1 + ((2/r)*dist) + ((1/r*r)*(dist*dist)));
		
		// diffuse
		NdotLPoint = max(dot(shadingNormal, -LightDir), 0);

		// specular
		halfPoint = normalize(-LightDir + view);

		// Instead of an If condition, it is sensible to mulitply specular term with cosine term
		// Ref : Natty Hoffman : SIGGRAPH 2010 ( Physically based shading ) 
		// if(NdotLPoint > 0)
			SpecPoint = BlinnBRDF(shadingNormal, halfPoint);// * NdotLPoint;

		// accumulate...
		DiffusePoint += pointLights[i].color * atten * NdotLPoint * pointLights[i].intensity;
		SpecularPoint += pointLights[i].color * atten * SpecPoint * pointLights[i].intensity;
	}

	// calculate reflection vector for environment mapping..
	vec3 R = reflect(Eye, envMapNormal);
	//vec4 reflectionColor = vec4(textureLod(texture_cubeMap, R, 9.0f));
	vec4 reflectionColor = vec4(texture(texture_cubeMap, R));

	// Final Color components...
	Emissive		= baseMapColor * material.Color;
	Ambient			= vec4(0.2, 0.2, 0.2, 1);
	Diffuse			= DiffuseDir + DiffusePoint;
	Specular		= SpecularDir + SpecularPoint; 
	//Reflection		= reflectionColor * material.reflectionColor;

	outColor = Emissive * (Ambient + Diffuse) + Specular;// + specMapColor;// * Reflection; 

	// linear depth
	/*float near = 0.1f;
	float far = 10.0f;

	float z = gl_FragCoord.z * 2 - 1;
	float depth = (2 * near) / (far + near - z * (far-near));

	outColor = vec4(vec3(1- depth), 1);*/
}