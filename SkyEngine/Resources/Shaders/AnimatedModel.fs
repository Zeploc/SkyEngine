#version 450 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D tex;
uniform int bIsLit = 1;


out vec4 color;


// ### LIGHT ###
uniform float ambientStr = 0.4f;
uniform vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightPos;// = vec3(-5.0f, 5.0f, 0.5f);

// Specular
uniform float lightSpecStr = 1.0f;
uniform vec3 camPos = vec3(1.0f, 1.0f, 3.0f);
uniform float shininess = 32.0f;

void SetLighting();

void main(void)
{	
	color = texture(tex, TexCoord);
	
	if (bIsLit == 1)
	{
		SetLighting();
	}
} 

void SetLighting()
{
	// Light Direction
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(FragPos.xyz - lightPos);
	
	// Specular Highlight
	vec3 negViewDir = normalize(camPos - FragPos.xyz);
	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(dot(negViewDir , reflectDir), 0.0f), shininess);
	vec3 specular = lightSpecStr * spec * lightColor;


	vec3 ambient = ambientStr * ambientColor;
	
	
	// Diffuse Coloring
	float diffuseStr = max(dot(norm, -lightDir), 0.0f);
	vec3 diffuse = diffuseStr * lightColor;
	
	color = color * vec4(ambient + diffuse + specular, 1.0f);
}