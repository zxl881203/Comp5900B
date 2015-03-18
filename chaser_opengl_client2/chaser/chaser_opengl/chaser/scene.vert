#version 330 core

uniform mat4 modelview; 
uniform mat4 projection;

in vec4 vertex;
in vec4 color;

out Data{
	vec4 color;
} Out;

void main(){
	gl_Position = projection * modelview * vertex;
	Out.color = color;
}