
#version 400

in vec3 vs_outTex;
out vec4 outColor;

uniform samplerCube cubeMap;

void main()
{
	outColor = texture(cubeMap, vs_outTex);
}
