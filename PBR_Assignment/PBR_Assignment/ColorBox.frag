#version 330 core

struct Material
{

sampler2D diffuse;
sampler2D specular;
sampler2D emission;
float shininess;
};
struct Light
{
vec3 position;

vec3 ambient;
vec3 diffuse;
vec3 specular;
};

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
uniform float time;

uniform Light light;  
uniform Material material;


uniform vec3 viewPosition;

void main()
{
	//ambient
 
 vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	 
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
	 vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb; 
    // specular        
	
	vec3 viewDir = normalize(viewPosition - FragPos);
	vec3 reflectDir= reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	  vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb; 
	  vec3 emission = vec3(0.0);
	  if (texture(material.specular, TexCoords).r == 0.0)   /*rough check for blackbox inside spec texture */
    {
	 emission = texture(material.emission, TexCoords).rgb;

	 //Moving and fading
	 vec3 emissionColor= vec3(0.0f,0.0f,1.0f);
	 emission = texture(material.emission, TexCoords+vec2(0.0,time)).rgb; // move
	 emission = emission *(sin(time)*0.5+0.7)*4; // fade

	}
	
    vec3 result = ( ambient+diffuse+specular+emission);
    FragColor = vec4(result, 1.0);
}
