
#version 400

in vec3 vs_outPosition;
in vec4 color;

layout (location = 0) out vec4 positionColor;
layout (location = 2) out vec4 albedoColor;

void main()
{
	positionColor = vec4(vs_outPosition, 1.0f);
	albedoColor = color;

	// check whether fragment color is more than the threshold brightness value
	// we calculate first grayscale equivalent...
	//float brightness = dot(outColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
	//if(brightness > 1.0f)
	//	brightColor = vec4(outColor.rgb, 1.0f);
}