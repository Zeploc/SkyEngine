#version 330 core

//in GS_FS_VERTEX{
//	vec2 texcoord;
//}fs_in;

//uniform sampler2D Texture;
uniform vec4 Colour;
out vec4 color;

in float lifetime;

void main()
{
	//color = texture(Texture, vec2(fs_in.texcoord.x , fs_in.texcoord.y)) * Colour;
	color = vec4(0.0f, 0.48f, 0.58f, lifetime);
	//color.z = lifetime;
} 