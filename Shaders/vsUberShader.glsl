
#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_Tex;

out vec2 vs_outTex;
out vec3 vs_outNormal;
out vec4 vs_outObjColor;
out vec3 vs_outPosition;

// uniforms...
uniform mat4 matWorld;
uniform mat4 matView;
uniform mat4 matProj;
uniform mat4 matWorldInv;

vec4 objColor = vec4(1,0,0,1);

void main()
{	
	mat4 WVP = matProj * matView * matWorld;
	gl_Position = WVP * vec4(in_Position, 1.0);

	// world space position
	vs_outPosition = (matWorld * vec4(in_Position, 1.0)).xyz;

	// Or take Light direction to Object space before lighting...
	vs_outNormal = normalize((matWorld * vec4(in_Normal,0)).xyz);

	vs_outTex = in_Tex;
	vs_outObjColor = objColor;
}