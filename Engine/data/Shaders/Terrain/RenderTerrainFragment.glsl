#version 440
uniform vec4 lightAmbientColor;
uniform vec4 lightDiffuseColor;
uniform vec4 lightSpecularColor;

uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform float shininess;


smooth in vec3 Normal;
smooth in vec3 vecYeux;
smooth in vec3 DirLum;

out vec4 color;

void main()
{
	vec3 N = normalize(Normal); 
	vec3 L = normalize(DirLum);

	color = clamp(materialAmbient * lightAmbientColor, 0.0, 1.0);
	float lambert = dot(N,L);
	if(lambert > 0.0)
	{
		color += clamp( materialDiffuse * lambert * lightDiffuseColor,0.0,1.0);
		vec3 E = normalize(vecYeux);
		vec3 R = reflect(-L,N);
		float specular = pow(max(dot(R,E),0.0), shininess) * 0.5;
		color += clamp(materialSpecular * specular * lightSpecularColor , 0.0,1.0);
	}
}