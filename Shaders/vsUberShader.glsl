
#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec3 in_Tangent;
layout(location=3) in vec3 in_Binormal;
layout(location=4) in vec2 in_Tex;

out vec2				vs_outTex;
out vec3				vs_outNormal;
out vec3				vs_outTangent;
out vec3				vs_outBinormal;
out vec3				vs_outPosition;

// uniforms...
uniform mat4		matWorld;
uniform mat4		matView;
uniform mat4		matProj;
uniform mat4		matWorldInv;

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
	vs_outTangent = in_Tangent;
	vs_outBinormal = in_Binormal;

	vs_outTex = in_Tex;
}