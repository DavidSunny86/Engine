#version 440 core

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);
in vec3 normal;
in vec3 modelNormal;
in vec3 DirLum;
in vec3 vecYeux;
in vec2 UV;
in vec4 shadowCoord;
out vec4 color;

uniform vec4 lightAmbientColor;
uniform vec4 lightDiffuseColor;
uniform vec4 lightSpecularColor;
uniform float lightIntensity;
uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform vec4 materialEmission;
uniform float shininess;

uniform sampler2DShadow shadowMap;
uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

uniform ivec2 viewPort;

uniform float reflectionPerturbationFactor;

void main()
{	
	vec3 N = normalize(normal);
	vec3 L = normalize(DirLum);

	float cosTheta = clamp( dot( N,L ), 0,1 );
	float bias = clamp(0.005 * tan(acos(cosTheta)),0.0,0.1);
	float visibility = 1.0;
	for (int i=0;i<16;++i){
		visibility -= 0.04*(1.0 - texture( shadowMap, vec3(shadowCoord.xy + poissonDisk[i]/1000.0,  (shadowCoord.z-bias)/shadowCoord.w) ));
	}
	vec2 screenSpace = vec2(gl_FragCoord.x / viewPort.x, gl_FragCoord.y/ viewPort.y);

	float distance = length(DirLum);
	float distanceSquared = distance * distance;

	color = clamp(visibility * materialAmbient * lightAmbientColor * lightIntensity / distanceSquared, 0.0, 1.0);


	float lambert = dot(N,L);
	if(lambert > 0.0)
	{
		vec4 diffuseFactor = visibility * materialDiffuse * lambert * lightDiffuseColor * lightIntensity / distanceSquared;
		color += clamp((diffuseFactor *  texture(refractionTexture,screenSpace + N.yz * 0.05)),0.0,1.0);
		color += clamp((diffuseFactor * texture(reflectionTexture,screenSpace - modelNormal.xz * reflectionPerturbationFactor)),0.0,1.0);
		vec3 E = normalize(vecYeux);
		vec3 R = reflect(-L,N);
		float specular = pow(max(dot(R,E),0.0), shininess) * 0.5;
		color += clamp((visibility * materialSpecular * specular * lightSpecularColor * lightIntensity) / distanceSquared, 0.0,1.0);
	}
}