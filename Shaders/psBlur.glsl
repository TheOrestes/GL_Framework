
#version 400 core

in vec2 vs_outTexCoord;

layout (location = 0) out vec4 bloomBlendColor;

uniform sampler2D screenTexture;
uniform sampler2D brightTexture;
uniform bool horizontal;
uniform bool doBloom;

uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541,
								   0.0162162162);

void main()
{
	vec2 tex_offset = vec2(1.0 / 2048.0);
	vec3 brightBlur = texture(brightTexture, vs_outTexCoord).rgb * weight[0];
	vec4 sceneColor = texture(screenTexture, vs_outTexCoord);

	if(doBloom)
	{
		if(horizontal)
		{
			for (int i = 1; i < 5; ++i)
			{
				brightBlur += texture(brightTexture, vs_outTexCoord + vec2(tex_offset.x * i, 0)).rgb * weight[i];
				brightBlur += texture(brightTexture, vs_outTexCoord - vec2(tex_offset.x * i, 0)).rgb * weight[i];
			}
		
		}
		else
		{
			for (int i = 1; i < 5; ++i)
			{
				brightBlur += texture(brightTexture, vs_outTexCoord + vec2(0, tex_offset.y * i)).rgb * weight[i];
				brightBlur += texture(brightTexture, vs_outTexCoord - vec2(0, tex_offset.y * i)).rgb * weight[i];
			}
		}

		bloomBlendColor = vec4(brightBlur, 1) + sceneColor;
	}
	else
	{
		bloomBlendColor = sceneColor;
	}
}