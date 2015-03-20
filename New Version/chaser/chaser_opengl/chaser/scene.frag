#version 330 core

in Data{
	vec4 color;
}In;

out vec4 color;

void main(){
	color = In.color; 
}