#version 450 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 MVP; 
uniform vec4 DiffuseColour; 

out vec4 PixelColor;
out vec2 PixelTextureCoord;
out vec3 PixelPosition;
out vec3 PixelNormal;

uniform mat4 model;

void main(void)
{
	gl_Position = MVP * vec4(in_position, 1.0);
	PixelPosition = vec3(model * vec4(in_position, 1.0f));
	PixelNormal = mat3(transpose(inverse(model))) * normal;
	PixelColor = DiffuseColour;
	PixelTextureCoord = texCoord;
} 