
#version 400

in vec2 vs_outTexCoord;

out vec4 outColor;

uniform sampler2D screenTexture;
uniform sampler2D blurTexture;
uniform bool bloomEnabled;

const float offset = 1.0f/3000.0f;
const vec2 offsets[9] = vec2[] (
								vec2(-offset, offset),
								vec2(0, offset),
								vec2(offset, offset),
								vec2(-offset, 0),
								vec2(0,0),
								vec2(offset, 0),
								vec2(-offset, -offset),
								vec2(0, -offset),
								vec2(offset, -offset)
							);

/////////////////////////////////////// Black-n-White ////////////////////////////////////////////
vec4 BlackWhiteFilter()
{
	vec4 screenColor = vec4(texture(screenTexture, vs_outTexCoord));
	return vec4((0.2126f * screenColor.r + 0.7152f * screenColor.g + 0.0722f * screenColor.b) );
}

////////////////////////////////////////// Blur ////////////////////////////////////////////
vec4 EdgeDetectionFilter(vec3 inSample[9])
{
	float kernel[9] = float[] (
								 1, 1, 1,
								 1, -8, 1,
								 1, 1, 1 
							  );

	vec3 color;
	for (int i = 0; i < 9; i++)
	{
		color += inSample[i] * (kernel[i]);
	}

	return vec4(color,1);
}

//////////////////////////////////////////// Sharpening /////////////////////////////////////////////////
vec4 SharpenFilter(vec3 inSample[9])
{
	float kernel[9] = float[] (
								-1,-1,-1,
								-1, 9,-1,
								-1,-1,-1 
							  );

	vec3 color;
	for (int i = 0; i < 9; i++)
	{
		color += inSample[i] * kernel[i];
	}

	return vec4(color,1);
}

//////////////////////////////////////////// Blur /////////////////////////////////////////////////
vec4 BlurFilter(vec3 inSample[9])
{
	float kernel[9] = float[] (
								1, 2, 1,
								2, 4, 2,
								1, 2, 1							  );

	vec3 color;
	for (int i = 0; i < 9; i++)
	{
		color += inSample[i] * (kernel[i]/16);
	}

	return vec4(color,1);
}

//////////////////////////////////////////// Laplace /////////////////////////////////////////////////
vec4 LaplaceFilter(vec3 inSample[9])
{
	float kernel[9] = float[] (
								 0, 1, 0,
								 1, -4,1,
								 0, 1, 0 
							  );
	vec3 color;
	for (int i = 0; i < 9; i++)
	{
		color += inSample[i] * kernel[i];
	}

	return vec4(color,1);
}

///////////////////////////////////////////// Emboss /////////////////////////////////////////////////
vec4 EmbossFilter(vec3 inSample[9])
{
	float kernel[9] = float[] (
								 2, 0, 0,
								 0,-1, 0,
								 0, 0, -1 
							  );
	vec3 color;
	for (int i = 0; i < 9; i++)
	{
		color += inSample[i] * kernel[i];
	}

	return vec4(color,1);
}

void main()
{
	// Sample current pixel with the offset from current location
	/*vec3 pixSample[9];
	for (int i = 0; i < 9; i++)
	{
		pixSample[i] = vec3(texture(screenTexture, vs_outTexCoord.st + offsets[i]));
	}*/

	//outColor = EdgeDetectionFilter(pixSample);
	//outColor = BlackWhiteFilter();


	vec3 hdrColor = vec3(texture(screenTexture, vs_outTexCoord));

	if(bloomEnabled)
	{
		vec3 bloomColor = texture(blurTexture, vs_outTexCoord).rgb;
		hdrColor += bloomColor;	
	}

	// Tone Mapping...
	float exposure = 1.0f;
	vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

	// Gamma Correction...
    vec3 retColor = pow(result, vec3(1/2.2));
    outColor = vec4(retColor,1);
}
