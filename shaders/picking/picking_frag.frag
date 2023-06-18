#version 330 core

out vec4 color;

uniform vec3 pickingColor;

void main (){

    color = vec4(pickingColor, 1);

}