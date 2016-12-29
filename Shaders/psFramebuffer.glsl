
#version 400

in vec2 vs_outTexCoord;

out vec4 outColor;

uniform sampler2D screenTexture;
uniform sampler2D brightTexture;
uniform int bloomEnabled;
uniform float exposure;
uniform int nBloomSamples;
uniform vec2 resolution;

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
vec4 BlackWhiteFilter(vec4 inColor)
{
	//vec4 screenColor = vec4(texture(screenTexture, vs_outTexCoord));
	return vec4((0.2126f * inColor.r + 0.7152f * inColor.g + 0.0722f * inColor.b) );
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

vec3 BloomBlur()
{
	float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

	vec2 tex_offset = vec2(1.0 / resolution.x, 1.0 / resolution.y);
	vec3 result = texture(brightTexture, vs_outTexCoord).rgb;
	result *= weight[0];

	int j,k = 0;

	for (int i = 1; i < nBloomSamples*2; i++)
	{
		if(i%2 == 0)
		{
			result += texture(brightTexture, vs_outTexCoord + vec2(tex_offset.x * j, 0)).rgb * weight[j];
			result += texture(brightTexture, vs_outTexCoord - vec2(tex_offset.x * j, 0)).rgb * weight[j];
			j++;
		}
		else
		{
			result += texture(brightTexture, vs_outTexCoord + vec2(0, tex_offset.y * k)).rgb * weight[k];
			result += texture(brightTexture, vs_outTexCoord - vec2(0, tex_offset.y * k)).rgb * weight[k];
			k++;
		}
	}

	return result;
}

void main()
{
	// Sample current pixel with the offset from current location
	/*vec3 pixSample[9];
	for (int i = 0; i < 9; i++)
	{
		pixSample[i] = vec3(texture(screenTexture, vs_outTexCoord.st + offsets[i]));
	}

	outColor = EdgeDetectionFilter(pixSample);
	outColor = BlackWhiteFilter(pixSample);*/
	vec3 hdrColor = vec3(texture(screenTexture, vs_outTexCoord));

	if(bloomEnabled > 0)
	{
		vec3 bloomColor = BloomBlur();
		hdrColor += bloomColor;	
	}

	// Tone Mapping...
	//float exposure = 1.2f;
	hdrColor *= exposure;

	vec3 result = hdrColor; 

	// Gamma Correction...
    vec3 retColor = pow(result, vec3(1/2.2));
    outColor = vec4(retColor,1);
}
