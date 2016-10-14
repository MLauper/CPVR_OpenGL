// simple modeling fragment shader code (core profile)

#version 400 

in vec4 color;

out vec4 fragColor; // fragment’s final color

void main()
{
	fragColor = color;
}

