
#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_Tex;

out vec2 vs_outTex;
out vec3 vs_outNormal;
out vec4 vs_outObjColor;
out vec3 vs_outPosition;
out vec3 vs_outEye;
out vec3 vs_outLightDir;

// uniforms...
uniform mat4		matWorld;
uniform mat4		matView;
uniform mat4		matProj;
uniform mat4		matWorldInv;
uniform vec3		camPosition;

uniform vec3		LightPosition;		// light position  
uniform vec3		Kd;					// Diffuse reflectivity
uniform vec3		Ld;					// light source intensity

vec4 objColor = vec4(1,0,0,1);

void main()
{	
	mat4 WVP = matProj * matView * matWorld;
	gl_Position = WVP * vec4(in_Position, 1.0);

	// world space position
	vs_outPosition = (matWorld * vec4(in_Position, 1.0)).xyz;

	// calculate camera eye vector in world space
	vs_outEye = normalize(vs_outPosition - camPosition);

	// lights in world space, calculate direction
	vs_outLightDir = normalize(vs_outPosition - LightPosition);

	// normals in world space
	vs_outNormal = normalize((matWorld * vec4(in_Normal,0)).xyz);

	vs_outTex = in_Tex;

	float NdotL = clamp(dot(vs_outNormal, -vs_outLightDir), 0, 1);

	vs_outObjColor = vec4(NdotL, NdotL, NdotL, 1);
}