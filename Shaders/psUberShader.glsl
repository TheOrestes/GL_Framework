
#version 400

in vec2 vs_outTex;
in vec3 vs_outNormal;
in vec4 vs_outObjColor;
in vec3 vs_outPosition;

out vec4 outColor;

uniform sampler2D	texture_diffuse1;
uniform sampler2D	texture_specular1;
uniform vec3		camPosition;

vec3 LightDirection = vec3(0,0,-1);

void main()
{
	vec4 baseColor = vec4(texture(texture_diffuse1, vs_outTex));
	vec4 specColor = vec4(texture(texture_specular1, vs_outTex));
		
	float NdotL = max(dot(vs_outNormal, -LightDirection), 0);

	// calculate view vector
	vec3 view = normalize(camPosition - vs_outPosition);

	// reflection vector
	vec3 refl = normalize(reflect(LightDirection, vs_outNormal));

	float VdotR = pow(max(dot(view, refl), 0), 32);

	vec4 Emissive = baseColor;
	vec4 Ambient = vec4(0.4, 0.4, 0.4, 1);
	vec4 DiffuseDirect = vec4(NdotL, NdotL, NdotL, 1);
	vec4 SpecularDirect = vec4(VdotR, VdotR, VdotR, 1);

	outColor = Emissive * (Ambient + DiffuseDirect) + specColor * SpecularDirect; 
}