
#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_Tex;

out vec2 vs_outTex;
out vec3 vs_outNormal;
out vec3 vs_outLightDir;
out vec4 vs_outObjColor;

// uniforms...
uniform mat4 matWorld;
uniform mat4 matView;
uniform mat4 matProj;
uniform mat4 matWorldInv;

vec3 LightDirection = vec3(0,-1,-1);
vec4 objColor = vec4(1,0,0,1);

void main()
{	
	mat4 WVP = matProj * matView * matWorld;
	gl_Position = WVP * vec4(in_Position, 1.0);

	// Either take object space normals to world space before lighting... 
	//vs_outNormal = normalize((matWorld * vec4(in_Normal, 0.0)).xyz);
	//vs_outLightDir = normalize(LightDirection);

	// Or take Light direction to Object space before lighting...
	vs_outNormal = normalize(in_Normal);
	vs_outLightDir = normalize((matWorldInv * vec4(LightDirection,0.0)).xyz);

	vs_outTex = in_Tex;
	vs_outObjColor = objColor;
}