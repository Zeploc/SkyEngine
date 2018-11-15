#version 450 core

// ## TEXTURE ###
out vec4 color;

in vec4 fragcolor;
in vec2 fragTexCoord;
in vec3 fragNormal;

uniform sampler2D tex;
uniform int bIsTex = 1;
uniform int bIsLit = 1;

// ### FOG ###
in vec4 mWorldPos;

uniform int bFog = 0;
uniform vec4 vFogColor;
uniform vec3 cameraPos;

uniform float StartFog = 5.0f;
uniform float EndFog = 10.0f;

// ### LIGHT ###
uniform int ToonShade = 0;
uniform float ambientStr = 0.4f;
uniform vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightPos;// = vec3(-5.0f, 5.0f, 0.5f);

// Specular
uniform float lightSpecStr = 1.0f;
uniform vec3 camPos = vec3(1.0f, 1.0f, 3.0f);
uniform float shininess = 32.0f;

void SetFogColour();
void SetLighting();

void main(void)
{	
	if (bIsTex == 1) color = texture(tex, fragTexCoord) * fragcolor;
	else color = fragcolor;
	
	if (bIsLit == 1)
	{
		SetLighting();
	}
	
	if (bFog == 1)
	{
		SetFogColour();
	}
	
} 

void SetFogColour()
{
	float d = distance(mWorldPos.xyz, cameraPos);
	float lerp = (d - StartFog)/EndFog;
	lerp = clamp(lerp, 0.0, 1.0);
	color = mix(color, vFogColor, lerp);
}

void SetLighting()
{
	// Light Direction
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(mWorldPos.xyz - lightPos);
	
	// Specular Highlight
	vec3 negViewDir = normalize(camPos - mWorldPos.xyz);
	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(dot(negViewDir , reflectDir), 0.0f), shininess);
	vec3 specular = lightSpecStr * spec * lightColor;


	vec3 ambient = ambientStr * ambientColor;
	
	
	// Diffuse Coloring
	float diffuseStr = max(dot(norm, -lightDir) * 2, 0.0f);
	vec3 diffuse = diffuseStr * lightColor;
	
	if (ToonShade == 1)
	{	
		if (diffuseStr > 0.95)
			diffuse = vec3(1.0,0.5,0.5);
		else if (diffuseStr > 0.5)
			diffuse = vec3(0.6,0.3,0.3);
		else if (diffuseStr > 0.25)
			diffuse = vec3(0.4,0.2,0.2);
		else
			diffuse = vec3(0.2,0.1,0.1);
	}
	float LightStrength = 100;
	float distanceFromLight = LightStrength / distance(mWorldPos.xyz, lightPos);
	diffuse = diffuse * lightColor;
	
	color = color * vec4(ambient + diffuse + specular, 1.0f);
}

