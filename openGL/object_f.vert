#version 330 core
struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

 

out vec4 FragColor;


uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;


in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

void main()
{
	//ambient light
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	//diffuse light
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb; 	
	//Specular Light 
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  


	//Total
    vec3 result = (ambient + diffuse + specular  );
    FragColor = vec4(result, 1.0);
}