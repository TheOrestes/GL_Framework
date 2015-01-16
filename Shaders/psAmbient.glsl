
#version 400

in vec2 vs_outTex;

out vec4 outColor;

uniform sampler2D texture_diffuse1;

void main()
{
	outColor = vec4(texture(texture_diffuse1, vs_outTex));
}