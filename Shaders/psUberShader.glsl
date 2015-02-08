
#version 400

in vec2 vs_outTex;
in vec3 vs_outNormal;
in vec4 vs_outObjColor;
in vec3 vs_outPosition;
in vec3 vs_outEye;
in vec3 vs_outLightDir;

out vec4 outColor;

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
		
	float NdotL = max(dot(vs_outNormal, -vs_outLightDir), 0);

	// calculate view vector
	vec3 view = -vs_outEye;

	// half vector
	vec3 half = normalize(-vs_outLightDir + view);

	// reflection vector
	vec3 refl = normalize(reflect(vs_outLightDir, vs_outNormal));


	// Final Color components...
	vec4 Emissive			= baseColor;
	vec4 Ambient			= vec4(0.4, 0.4, 0.4, 1);
	vec4 DiffuseDirect		= vec4(NdotL, NdotL, NdotL, 1);
	vec4 SpecularDirect = vec4(0); 
	
	if(NdotL > 0.0)
	{
		SpecularDirect += PhongBRDF(vs_outNormal, view, refl);
		//SpecularDirect += BlinnBRDF(vs_outNormal, view, half);
	    //SpecularDirect += CookTorranceBRDF(vs_outNormal, view, normalize(-vs_outLightDir), half);
	}

	//outColor = Emissive * (Ambient + DiffuseDirect) + specColor * SpecularDirect; 

	outColor = baseColor * vs_outObjColor;
}