#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoords;

out vec4 FragColor;

uniform vec3 cameraPosition;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform float lightIntensity;

uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float shininess;

uniform sampler2D texture_diffuse;

void main()
{

    //Ambient component
    vec3 ambientComponent = materialAmbient * (lightColor * lightIntensity);

    //Diffuse component
    vec3 normal = normalize(fragNormal);
    vec3 lightDirection = normalize(lightPosition - fragPosition);
    float cosTheta = max(dot(normal, lightDirection), 0.0);
    vec3 diffusiveComponent = materialDiffuse * (lightColor * lightIntensity) * cosTheta;
    
    //Specular component
    vec3 specularComponent = vec3(0,0,0);

	vec3 view_dir    = normalize(fragPosition - cameraPosition);
	vec3 reflect_dir = normalize(reflect(lightDirection, normal));

	float cosAlpha = dot(view_dir,reflect_dir);
	if (cosAlpha>0) {
		specularComponent = materialSpecular * (lightColor * lightIntensity) * pow(cosAlpha, shininess);
	}
    //Calcolo del colore finale del fragment
    //FragColor = vec4(ambientComponent + diffusiveComponent, 1);
    //FragColor = vec4(1, 1, 0, 1);

    FragColor = texture(texture_diffuse, fragTexCoords);



}