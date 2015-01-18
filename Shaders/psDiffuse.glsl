
#version 400

in vec2 vs_outTex;
in vec3 vs_outNormal;
in vec3 vs_outLightDir;
in vec4 vs_outObjColor;

out vec4 outColor;

uniform sampler2D texture_diffuse1;

void main()
{
	vec4 baseColor = vec4(texture(texture_diffuse1, vs_outTex));
		
	float NdotL = max(dot(vs_outNormal, -vs_outLightDir), 0);

	vec4 Emissive = baseColor;
	vec4 Ambient = vec4(0.2, 0.2, 0.2, 1);
	vec4 DiffuseDirect = vec4(NdotL);

	outColor = Emissive * (Ambient + DiffuseDirect);
}