
#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec3 in_Tangent;
layout(location=3) in vec3 in_Binormal;
layout(location=4) in vec2 in_Tex;

out vec2			vs_outTex;
out vec3			vs_outNormal;
out vec3			vs_outPosition;
out mat3			vs_outTBN;

// uniforms...
uniform mat4		matWorld;
uniform mat4		matView;
uniform mat4		matProj;

//---------------------------------------------------------------------------------------
// Main Shader Code
//---------------------------------------------------------------------------------------
void main()
{	
	mat4 WVP = matProj * matView * matWorld;
	gl_Position = WVP * vec4(in_Position, 1.0);

	// world space position
	vs_outPosition = (matWorld * vec4(in_Position, 1)).xyz;

	// normals in world space
	vs_outNormal = (matWorld * vec4(in_Normal, 0)).xyz;

	// output rest of the stuff for future use..
	vec3 T = normalize(vec3(matWorld * vec4(in_Tangent, 0)));
	vec3 B = normalize(vec3(matWorld * vec4(in_Binormal, 0)));
	vec3 N = normalize(vec3(matWorld * vec4(in_Normal, 0)));
	vs_outTBN = mat3(T,B,N);

	vs_outTex = in_Tex;
}