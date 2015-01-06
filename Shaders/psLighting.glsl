
#version 400

//in vec3 position;
in vec3 Normal;

out vec4 outColor;

uniform vec4 objectColor;
uniform vec3 lightDir;

void main()
{
	// diffuse component
	float diffuse =  max(dot(Normal, lightDir), 0);
	outColor = diffuse * objectColor;
}