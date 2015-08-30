
#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;

uniform mat4 matWorld;
uniform mat4 matView;
uniform mat4 matProj;
uniform vec3 lightDir;

out vec3 outPosition;
out vec3 Normal;
out vec4 Color;

void main()
{
	mat4 WVP = matProj * matView * matWorld;
	gl_Position = WVP * vec4(in_Position, 1.0);

	//outPosition = (matWorld * vec4(in_Position, 1.0)).xyz;
	Normal = normalize(matWorld * vec4(in_Normal, 0)).xyz;

	outPosition = (matWorld * vec4(in_Position, 1.0)).xyz;
	//Normal = in_Normal;
	float NdotL = max(dot(Normal, vec3(1,1,0)), 0);
	Color = vec4(NdotL, NdotL, NdotL, 1);
}