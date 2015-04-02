
#version 400

in vec2 vs_outTexCoord;

out vec4 outColor;

uniform sampler2D screenTexture;

void main()
{
	vec4 frameBuffer = vec4(texture(screenTexture, vs_outTexCoord));
	outColor = vec4((frameBuffer.r + frameBuffer.g + frameBuffer.b) / 3);
}
