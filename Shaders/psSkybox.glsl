
#version 400

in vec3 vs_outTex;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 brightColor;

uniform samplerCube cubeMap;

void main()
{
	outColor = texture(cubeMap, -vs_outTex);

	// check whether fragment color is more than the threshold brightness value
	// we calculate first grayscale equivalent...
	float brightness = dot(outColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
	if(brightness > 200.0f)
		brightColor = vec4(outColor.rgb, 1.0f);
}
