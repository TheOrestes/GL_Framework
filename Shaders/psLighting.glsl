
#version 400

in vec3 outPosition;
in vec3 Normal;
in vec4 Color;

out vec4 outColor;

uniform vec4 objectColor;
uniform vec3 lightDir;

void main()
{
	// diffuse component
	float diffuse =  max(dot(Normal,  vec3(1,1,0)), 0);
	outColor = vec4(diffuse, diffuse, diffuse, 1);
}