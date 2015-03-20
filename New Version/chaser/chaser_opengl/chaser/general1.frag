#version 330 core

in Data{
	vec4 pos;
	vec4 normal;
	vec4 colour;
	vec2 tex;
} vIn;

out vec4 color;

void main()

{

	color = vIn.colour; 
}