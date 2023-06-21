#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoords;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0);

    fragPosition = vec3(model * vec4(position, 1.0));
    fragNormal = (transpose(inverse(model)) * vec4(normal, 0)).xyz;  
}