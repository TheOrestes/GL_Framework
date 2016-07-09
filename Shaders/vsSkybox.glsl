
#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec3 in_Tangent;
layout(location=3) in vec3 in_Binormal;
layout(location=4) in vec2 in_Tex;

out vec4 vs_outPosition;
out vec2 vs_outTex;

// uniforms
uniform mat4 matWorld;
uniform mat4 matView;
uniform mat4 matProj;

void main()
{
	// For our sky sphere, first take only 3x3 matrix from the view matrix,
	// this will remove the translation part & keep only rotation part intact.
	// then convert it back to 4x4 matrix. This way, skybox moves with player!
	mat3 view = mat3(matView);
	mat4 _view = mat4(view);
	mat4 wvp = matProj * _view * matWorld;
	vec4 Pos = wvp * vec4(in_Position, 1.0f);
	
	gl_Position = Pos.xyww; 
	vs_outPosition = Pos.xyww;
	vs_outTex = in_Tex;
}