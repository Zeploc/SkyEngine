#version 450 core

out vec4 color;

in vec4 PixelColor;
in vec2 PixelTextureCoord;
in vec3 PixelPosition;
in vec3 PixelNormal;

// TODO: Textures

uniform int bHasDiffuseTexture = 0;
uniform sampler2D DiffuseTexture;
uniform vec3 CameraPosition = vec3(1.0f, 1.0f, 3.0f);

// Fog
uniform vec4 LightColour;
uniform float StartFog = 20.0f;
uniform float EndFog = 40.0f;

void main(void)
{	
	// Light Direction
	vec3 Normal = normalize(PixelNormal);
	
	// Specular Highlight
	vec3 negViewDir = normalize(CameraPosition - PixelPosition);	
	
	if (bHasDiffuseTexture == 1) 
	{
	    color = texture(DiffuseTexture, PixelTextureCoord) * PixelColor;
	}
	else
	{
	    color = PixelColor;
	}
 
	// TODO: Link fog
// 	float d = distance(mWorldPos.xyz, cameraPos);
// 	float lerp = (d - 5.0f)/10.f;
// 	lerp = clamp(lerp, 0.0, 1.0);
// 	color = texture(tex, fragTexCoord) * fragcolor;
// 	color = mix(color, vFogColor, lerp);
} 

