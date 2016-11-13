
#version 400

//in vec4 vs_outPosition;
in vec2 vs_outTex;

//layout (location = 0) out vec4 positionColor;
layout (location = 2) out vec4 albedoColor;

uniform sampler2D texture_hdri;

void main()
{
	//positionColor = vs_outPosition;
	albedoColor = texture(texture_hdri, vs_outTex);

	// check whether fragment color is more than the threshold brightness value
	// we calculate first grayscale equivalent...
	//float brightness = dot(albedoColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
	//if(brightness > 200.0f)
	//	brightColor = vec4(albedoColor.rgb, 1.0f);
}
