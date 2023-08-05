#version 450 core

out vec4 color;

in vec4 PixelColor;
in vec2 PixelTextureCoord;
in vec3 PixelPosition;
in vec3 PixelNormal;

// TODO: Textures

uniform int bIsTex = 1;
uniform sampler2D tex;
uniform float AmbientStrength = 0.1f;
uniform vec3 LightColour = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 LightPosition;// = vec3(-5.0f, 5.0f, 0.5f);
uniform vec3 LightDirection;// = vec3(-5.0f, 5.0f, 0.5f);

// Specular
uniform float SpecularStrength = 1.0f;
uniform vec3 CameraPosition = vec3(1.0f, 1.0f, 3.0f);
uniform float Shininess = 32.0f;

// Fog
uniform float StartFog = 20.0f;
uniform float EndFog = 40.0f;

void main(void)
{
	 color = vec4(1,1,1,1);// PixelColor;
	 return;
 
	// Light Direction
	vec3 Normal = normalize(PixelNormal);
	vec3 LightDirection = normalize(PixelPosition - LightPosition);
	
	// Specular Highlight
	vec3 negViewDir = normalize(CameraPosition - PixelPosition);
	vec3 ReflectDirection = reflect(LightDirection, Normal);
	float spec = pow(max(dot(negViewDir, ReflectDirection), 0.0f), Shininess);
	vec3 specular = SpecularStrength * spec * LightColour;

	vec3 ambient = AmbientStrength * LightColour;	
	
	// Diffuse Coloring
	float diffuseStr = max(dot(Normal, -LightDirection), 0.0f);
	vec3 diffuse = diffuseStr * LightColour;
	
	if (bIsTex == 1) 
	{
	    color = vec4(ambient + diffuse + specular, 1.0f) * texture(tex, PixelTextureCoord) * PixelColor;
	}
	else
	{
	    color = vec4(ambient + diffuse + specular, 1.0f) * PixelColor;
	}
 
	// TODO: Link fog
// 	float d = distance(mWorldPos.xyz, cameraPos);
// 	float lerp = (d - 5.0f)/10.f;
// 	lerp = clamp(lerp, 0.0, 1.0);
// 	color = texture(tex, fragTexCoord) * fragcolor;
// 	color = mix(color, vFogColor, lerp);
} 

