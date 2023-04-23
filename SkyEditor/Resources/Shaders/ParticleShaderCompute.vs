#version 440 core

layout (location = 0) in vec4 position;

out float lifetime;
uniform mat4 vp;

void main()
{
	// movement is in world space
	gl_Position = vp * vec4(position.xyz, 1.0f);
	lifetime = position.w;
} 